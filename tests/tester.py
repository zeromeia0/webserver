import requests, pytest, subprocess, json, os

BASE_URL = "http://localhost:8089"

_requests = [
	{
		"uri": "/",
		"method": "GET",
		"body": "",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/",
		"method": "POST",
		"body": "",
		"expected": {
			"status": 405,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": "",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": f"{'e' * 100}",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": f"{'e' * 10000}",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": f"{'e' * 1000000}",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": f"{'e' * 10000000}",
		"expected": {
			"status": 200,
		},
	},
	{
		"uri": "/cgi-bin/test_cgi.py",
		"method": "POST",
		"body": f"{'e' * 100000000}",
		"expected": {
			"status": 413,
		},
	},
	{
		"uri": "/autoindexoff",
		"method": "GET",
		"expected": {
			"status": 403,
		},
	},
	{
		"uri": "/autoindexon",
		"method": "GET",
		"expected": {
			"status": 200,
			"content": "<h2>/autoindexon</h2>\n<div><a href=/autoindexon/. >.<a></div>\n<div><a href=/autoindexon/.. >..<a></div>\n<div><a href=/autoindexon/aurevoir.html >aurevoir.html<a></div>\n<div><a href=/autoindexon/bonjour.html >bonjour.html<a></div>\n"
		},
	},
]

def run_request(t):
	res = requests.request(t["method"], BASE_URL + t["uri"], data=t.get("body"))
	if (t["expected"].get("status")):
		assert res.status_code == t["expected"]["status"]
	if (t["expected"].get("content")):
		assert res.text == t["expected"]["content"]

@pytest.mark.parametrize("t", _requests, ids=lambda t: f"{t['method']} {t['uri']}")
def test_request(t):
	run_request(t)

_requests = [
	(
		"./tests/configsFiles/1.conf",
		"Error: Empty configuration file"
	),
	(
		"./tests/configsFiles/2.conf,,,,,,,,,,,,,,,,,",
		"Error: Bad extension"
	),
	(
		"./tests/configsFiles/3.co",
		"Error: Bad extension"
	),
	(
		"./tests/configsFiles/4",
		"Error: Bad extension"
	),
	(
		"./tests/configsFiles/5.conf",
		"Error: Binding socket :8091"
	),
	(
		"./tests/configsFiles/6.conf",
		"Error: Listen port out of range"
	),
]

def run_configs(dir):
	result = subprocess.run(
		["./webserv", dir],
		capture_output=True, text=True
	)
	assert "Error: Config file can not be opened" in result.stderr

@pytest.mark.parametrize("t", os.listdir("./tests/configsFiles"))
def test_configs(t):
	run_configs(t)

def test_siege():
	result = subprocess.run(
		["siege", "-c", "20", "-r", "50", BASE_URL + "/"],
		capture_output=True, text=True
	)
	assert 0 == json.loads(result.stdout)["failed_transactions"]

# def test_42():
# 	result = subprocess.run(
# 		["./tests/42tester/tester", "http://localhost:8888"],
# 		capture_output=True, text=True
# 	)
# 	assert "ERROR ON LAST TEST" not in result.stdout
