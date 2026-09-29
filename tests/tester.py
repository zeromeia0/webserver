import requests, pytest, subprocess, json, os, socket, asyncio

WEBSERV_URL = "http://localhost:8089"

###################################
# REQUESTS
###################################

from .requests import _requests

def run_request(t):
	res = requests.request(t["method"], WEBSERV_URL + t["uri"], data=t.get("body"))
	if (t["expected"].get("status")):
		assert res.status_code == t["expected"]["status"]
	if (t["expected"].get("content")):
		assert res.text == t["expected"]["content"]

@pytest.mark.parametrize("t", _requests, ids=lambda t: f"{t['method']} {t['uri']}")
def test_request(t):
	run_request(t)

###################################
# CONFIGS
###################################

CONFIGS_PATH = "./tests/configs/"
from .configs import _configs

def run_configs(t):
	try:
		result = subprocess.run(
			["./webserv", CONFIGS_PATH + t["file"]],
			capture_output=True, text=True, errors="replace", timeout=2
		)
	except subprocess.TimeoutExpired:
		assert t["exp"] is None, "server started but an error was expected"
		return
	assert t["exp"] is not None, "server exited: " + result.stderr
	assert t["exp"] in result.stderr

@pytest.mark.parametrize("t", _configs, ids=lambda t: f'{t["file"]} {t["exp"]}')
def test_configs(t):
	run_configs(t)

###################################
# NCS
###################################

from .ncs import _ncs

def run_nc(t):
	result = subprocess.run(
		["nc", "-q", "1", "localhost", "8089"],
		input=t["req"], capture_output=True, text=True, timeout=10
	)
	for e in t["exp"]:
		assert e in result.stdout, result.stdout[:300]
	for n in t.get("not", []):
		assert n not in result.stdout, result.stdout[:300]

@pytest.mark.parametrize("t", _ncs, ids=lambda t: repr(t["req"][:60]))
def test_nc(t):
	run_nc(t)

###################################
# SIEGE
###################################

def test_siege():
	result = subprocess.run(
		["siege", "-c", "20", "-r", "500", WEBSERV_URL + "/"],
		capture_output=True, text=True
	)
	assert 0 == json.loads(result.stdout)["failed_transactions"]

###################################
# 42
###################################

TESTER_PATH = "./tests/42/tester"
FORTYTWO_URL = "http://localhost:8888"

def test_42():
	result = subprocess.run(
		[TESTER_PATH, FORTYTWO_URL],
		stdin=subprocess.DEVNULL, capture_output=True, text=True
	)
	assert "FATAL ERROR" not in result.stdout, result.stdout[-2000:]
	assert "Test multiple workers(20) doing multiple times(5): Post on /directory/youpi.bla" in result.stdout, "tester did not finish"

###################################
# AI
###################################

def test_concurrent_methods():
	# #6b: a GET parsed before a POST's response is built must not change the POST's method
	for _ in range(30):
			b = socket.create_connection(("localhost", 8089))       # B first: lower index in the poll list
			a = socket.create_connection(("localhost", 8089))
			a.settimeout(5)
			a.sendall(b"POST /uploads/race.txt HTTP/1.1\r\nHost: x\r\nContent-Length: 2\r\n\r\nhi")
			b.sendall(b"GET / HTTP/1.1\r\nHost: x\r\n\r\n")
			res = b""
			while True:
					d = a.recv(4096)
					if not d:
							break
					res += d
			a.close(); b.close()
			requests.delete(WEBSERV_URL + "/uploads/race.txt")
			assert b"201 Created" in res, res.split(b"\r\n")[0]

def test_multiple_ports():
	a = requests.get("http://localhost:8089/")
	b = requests.get("http://localhost:8090/")
	c = requests.get("http://localhost:8091/")
	assert a.text == b.text
	assert c.text == open("./var/other/index.html").read()

def test_custom_error_page():
	r = requests.get(WEBSERV_URL + "/nope.html")
	assert r.status_code == 404
	assert r.text == open("./var/errors/404.html").read()

def test_multipart_upload():
	r = requests.post(WEBSERV_URL + "/uploads", files={"file": ("mp.txt", b"multipart body")})
	assert r.status_code == 201
	r = requests.get(WEBSERV_URL + "/uploads/mp.txt")
	assert r.status_code == 200 and r.text == "multipart body"
	assert requests.delete(WEBSERV_URL + "/uploads/mp.txt").status_code == 204

def test_encoded_space_roundtrip():
	assert requests.post(WEBSERV_URL + "/uploads/my%20file.txt", data="sp").status_code == 201
	assert os.path.exists("./var/uploads/my file.txt")
	r = requests.get(WEBSERV_URL + "/uploads/my%20file.txt")
	assert r.status_code == 200 and r.text == "sp"
	assert requests.delete(WEBSERV_URL + "/uploads/my%20file.txt").status_code == 204

def _up(name):
	return "./var/uploads/" + name

def test_upload_utf8_name_on_disk():
	# the name must be stored decoded as real UTF-8, not "%C3%A0" or mojibake
	assert requests.post(WEBSERV_URL + "/uploads/%C3%A0%20venda.txt", data="x").status_code == 201
	assert os.path.exists(_up("à venda.txt"))

def test_upload_special_names():
	for name, enc in (("50%.txt", "50%25.txt"), ("a#b.txt", "a%23b.txt"), ("a?b.txt", "a%3Fb.txt"),
										("l'appart.txt", "l%27appart.txt"), ("c++.txt", "c++.txt")):
			assert requests.post(WEBSERV_URL + "/uploads/" + enc, data="x").status_code == 201, name
			assert os.path.exists(_up(name)), name
			assert requests.get(WEBSERV_URL + "/uploads/" + enc).status_code == 200, name
			assert requests.delete(WEBSERV_URL + "/uploads/" + enc).status_code == 204, name

def test_multipart_utf8_name():
	r = requests.post(WEBSERV_URL + "/uploads", files={"file": ("é — x.txt", b"mp")})
	assert r.status_code == 201
	assert os.path.exists(_up("é — x.txt"))
	requests.delete(WEBSERV_URL + "/uploads/" + requests.utils.quote("é — x.txt"))
	assert requests.delete(WEBSERV_URL + "/uploads/%C3%A0%20venda.txt").status_code == 204

def test_multipart_traversal_name():
	# filename="../../evil.txt" must land INSIDE uploads, never outside
	r = requests.post(WEBSERV_URL + "/uploads", files={"file": ("../../evil.txt", b"evil")})
	assert r.status_code == 201
	assert os.path.exists(_up("evil.txt"))
	assert not os.path.exists("./var/evil.txt") and not os.path.exists("./evil.txt")
	requests.delete(WEBSERV_URL + "/uploads/evil.txt")
	
def test_multipart_two_files():
	r = requests.post(WEBSERV_URL + "/uploads", files=[("a", ("one.txt", b"1")), ("b", ("two.txt", b"2"))])
	assert r.status_code == 201
	assert requests.get(WEBSERV_URL + "/uploads/one.txt").text == "1"
	assert requests.get(WEBSERV_URL + "/uploads/two.txt").text == "2"
	requests.delete(WEBSERV_URL + "/uploads/one.txt"); requests.delete(WEBSERV_URL + "/uploads/two.txt")
	
def test_binary_roundtrip():
	data = bytes(range(256)) * 4            # every byte value, including \0 \r \n
	assert requests.post(WEBSERV_URL + "/uploads/bin.dat", data=data).status_code == 201
	assert requests.get(WEBSERV_URL + "/uploads/bin.dat").content == data
	assert requests.delete(WEBSERV_URL + "/uploads/bin.dat").status_code == 204
	
def test_overwrite():
	requests.post(WEBSERV_URL + "/uploads/ow.txt", data="first")
	assert requests.post(WEBSERV_URL + "/uploads/ow.txt", data="2nd").status_code == 201
	assert requests.get(WEBSERV_URL + "/uploads/ow.txt").text == "2nd"
	requests.delete(WEBSERV_URL + "/uploads/ow.txt")

def test_incomplete_body_times_out():
	# subject: "A request to your server should never hang indefinitely" (slow: ~31s)
	s = socket.create_connection(("localhost", 8089)); s.settimeout(45)
	s.sendall(b"POST /uploads/slow.txt HTTP/1.1\r\nHost: x\r\nContent-Length: 100\r\n\r\nonly-10-b")
	res = b""
	while True:
			d = s.recv(4096)
			if not d:
					break
			res += d
	assert b"408 Request Timeout" in res

def test_delete_directory_refused():
	r = requests.delete(WEBSERV_URL + "/uploads/")
	assert r.status_code != 204
	assert os.path.isdir("./var/uploads")