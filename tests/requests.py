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
			"content": "<h2>/autoindexon</h2>\n<div><a href=\"/autoindexon/.\">.</a></div>\n<div><a href=\"/autoindexon/..\">..</a></div>\n<div><a href=\"/autoindexon/aurevoir.html\">aurevoir.html</a></div>\n<div><a href=\"/autoindexon/bonjour.html\">bonjour.html</a></div>\n"
		},
	},
	{
		"uri": "/cgi-bin/timeout.js",
		"method": "GET",
		"expected": {
			"status": 200,
			"content": "Please update QUERY_STRING\nSuccessfuly waited for undefined seconds\n"
		},
	},
	{
		"uri": "/cgi-bin/timeout.js?time=1",
		"method": "GET",
		"expected": {
			"status": 200,
			"content": "\nSuccessfuly waited for 1 seconds\n"
		},
	},
	{
		"uri": "/cgi-bin/timeout.js?time=31",
		"method": "GET",
		"expected": {
			"status": 504,
		},
	},
]
