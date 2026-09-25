import requests, pytest, subprocess, json

BASE_URL = "http://localhost:8089"

tests = [
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
]

def run_test(t):
	res = requests.request(t["method"], BASE_URL + t["uri"], data=t["body"])
	assert res.status_code == t["expected"]["status"]

@pytest.mark.parametrize("t", tests, ids=lambda t: f"{t['method']} {t['uri']}")
def test_request(t):
	run_test(t)

def test_siege():
	result = subprocess.run(
		["siege", "-c", "20", "-r", "50", BASE_URL + "/"],
		capture_output=True, text=True
	)
	assert 0 == json.loads(result.stdout)["failed_transactions"]

if (__name__ == "__main__"):
	input("Make sure the webserver is up and running.\nPress Enter to continue")
	test_siege()
