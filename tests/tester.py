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
	{"file": "1.conf", "exp": "Error: Empty configuration file"},
	{"file": "2.conf,,,,,,,,,,,,,,,,,", "exp": "Error: Bad extension"},
	{"file": "3.co", "exp": "Error: Bad extension"},
	{"file": "4", "exp": "Error: Bad extension"},
	{"file": "5.conf", "exp": "Error: Binding socket :8091"},
	{"file": "6.conf", "exp": "Error: Invalid listen port: 809132423"},
	{"file": ".conf", "exp": "Error: Bad extension"},
	{"file": "07_whitespace_only.conf", "exp": "Error: Empty configuration file"},
	{"file": "08_comment_only.conf", "exp": "Error: Unknown directive: #"},
	{"file": "09_comment_inline.conf", "exp": "Error: Unknown directive: #"},
	{"file": "10_nul_byte.conf", "exp": "Error: Invalid listen port: 82"},
	{"file": "11_binary_garbage.conf", "exp": "Error: Unknown directive: "},
	{"file": "12_crlf_valid.conf", "exp": None},
	{"file": "13_huge_token.conf", "exp": None},
	{"file": "14_double.conf.conf", "exp": None},
	{"file": "15_unicode_directive.conf", "exp": "Error: Unknown directive: sérver_name"},
	{"file": "20_missing_close_brace.conf", "exp": "Error: Missing closing brace"},
	{"file": "21_extra_close_brace.conf", "exp": "Error: Extra closing brace"},
	{"file": "22_close_before_open.conf", "exp": "Error: Extra closing brace"},
	{"file": "23_server_no_brace.conf", "exp": "Error: Extra closing brace"},
	{"file": "24_server_semicolon.conf", "exp": "Error: server block must be followed by '{'"},
	{"file": "25_nested_server.conf", "exp": "Error: Server directive in wrong context: listen"},
	{"file": "26_nested_location.conf", "exp": "Error: location directive must be inside server block"},
	{"file": "27_location_outside_server.conf", "exp": "Error: location directive must be inside server block"},
	{"file": "28_directive_outside_server.conf", "exp": "Error: Server directive in wrong context: listen"},
	{"file": "29_stray_brace_block.conf", "exp": "Error: Server directive in wrong context: listen"},
	{"file": "30_deep_braces.conf", "exp": "Error: Missing server block"},
	{"file": "31_location_no_path_eof.conf", "exp": "Error: location directive must be inside server block"},
	{"file": "32_location_no_path.conf", "exp": "Error: Unknown directive: ./var/www"},
	{"file": "33_location_two_paths.conf", "exp": "Error: Invalid location block"},
	{"file": "34_location_path_no_slash.conf", "exp": "Error: Invalid location path"},
	{"file": "35_location_duplicate.conf", "exp": "Error: Duplicate location: /"},
	{"file": "36_empty_server.conf", "exp": "Error: Missing listen directive"},
	{"file": "37_second_server_no_listen.conf", "exp": "Error: Server block without listen directive"},
	{"file": "38_server_no_location.conf", "exp": "Error: Server block without location"},
	{"file": "39_empty_location.conf", "exp": "Error: Location needs root, alias or redirect: /"},
	{"file": "40_keyword_as_value.conf", "exp": "Error: server block must be followed by '{'"},
	{"file": "41_brace_as_value.conf", "exp": "Error: Missing closing brace"},
	{"file": "42_server_is_last_token.conf", "exp": "Error: server block must be followed by '{'"},
	{"file": "50_missing_semicolon.conf", "exp": "Error: Missing semicolon after directive: listen"},
	{"file": "51_missing_semicolon_before_brace.conf", "exp": "Error: Missing semicolon after directive: root"},
	{"file": "52_double_semicolon.conf", "exp": "Error: Unexpected ;"},
	{"file": "53_lone_semicolon.conf", "exp": "Error: Unexpected ;"},
	{"file": "54_directive_no_value.conf", "exp": "Error: Invalid number of arguments in directive: listen"},
	{"file": "55_listen_two_values.conf", "exp": "Error: Invalid number of arguments in directive: listen"},
	{"file": "56_unknown_directive.conf", "exp": "Error: Unknown directive: foo"},
	{"file": "57_typo_directive.conf", "exp": "Error: Unknown directive: lisen"},
	{"file": "58_uppercase_directive.conf", "exp": "Error: Unknown directive: LISTEN"},
	{"file": "59_directive_eof_no_semicolon.conf", "exp": "Error: Missing semicolon after directive: listen"},
	{"file": "60_listen_zero.conf", "exp": "Error: Invalid listen port: 0"},
	{"file": "61_listen_65536.conf", "exp": "Error: Listen port out of range"},
	{"file": "62_listen_negative.conf", "exp": "Error: Invalid listen port: -8200"},
	{"file": "63_listen_plus_sign.conf", "exp": "Error: Invalid listen port: +8200"},
	{"file": "64_listen_overflow_wraps.conf", "exp": "Error: Invalid listen port: 4294975496"},
	{"file": "65_listen_huge.conf", "exp": "Error: Invalid listen port: 99999999999999999999999999"},
	{"file": "66_listen_letters.conf", "exp": "Error: Invalid listen port: http"},
	{"file": "67_listen_empty_port.conf", "exp": "Error: Invalid listen port: "},
	{"file": "68_listen_bad_ip.conf", "exp": "Error: getaddrinfo: Name or service not known"},
	{"file": "69_listen_garbage_host.conf", "exp": "Error: getaddrinfo: Name or service not known"},
	{"file": "70_listen_two_colons.conf", "exp": "Error: Invalid listen port: 80:8200"},
	{"file": "71_listen_ipv6.conf", "exp": "Error: Invalid listen port: :1]:8200"},
	{"file": "72_listen_duplicate_same_server.conf", "exp": "Error: Binding socket :8200"},
	{"file": "73_listen_duplicate_two_servers.conf", "exp": "Error: Binding socket :8200"},
	{"file": "74_listen_privileged.conf", "exp": "Error: Binding socket :80"},
	{"file": "75_listen_float.conf", "exp": "Error: Invalid listen port: 8200.5"},
	{"file": "76_listen_hex.conf", "exp": "Error: Invalid listen port: 0x2008"},
	{"file": "77_listen_unresolvable_host.conf", "exp": "Error: getaddrinfo: Name or service not known"},
	{"file": "78_listen_in_location.conf", "exp": "Error: Server directive in wrong context: listen"},
	{"file": "80_cmbs_negative.conf", "exp": "Error: Invalid client_max_body_size"},
	{"file": "81_cmbs_unit.conf", "exp": "Error: Invalid client_max_body_size"},
	{"file": "82_cmbs_overflow.conf", "exp": "Error: client_max_body_size too large"},
	{"file": "83_cmbs_int_wrap.conf", "exp": "Error: client_max_body_size too large"},
	{"file": "84_cmbs_duplicate.conf", "exp": "Error: Duplicate directive: client_max_body_size"},
	{"file": "85_cmbs_empty.conf", "exp": "Error: Invalid number of arguments in directive: client_max_body_size"},
	{"file": "86_cmbs_float.conf", "exp": "Error: Invalid client_max_body_size"},
	{"file": "90_error_page_missing_path.conf", "exp": "Error: Invalid number of arguments in directive: error_page"},
	{"file": "91_error_page_code_99.conf", "exp": "Error: Invalid error code"},
	{"file": "92_error_page_code_600.conf", "exp": "Error: Error code out of range"},
	{"file": "93_error_page_code_letters.conf", "exp": "Error: Invalid error code"},
	{"file": "94_error_page_code_overflow.conf", "exp": "Error: Invalid error code"},
	{"file": "95_error_page_multi_codes.conf", "exp": "Error: Invalid number of arguments in directive: error_page"},
	{"file": "96_error_page_missing_file.conf", "exp": "Error: Error page is not a readable file: ./does/not/exist.html"},
	{"file": "97_error_page_is_dir.conf", "exp": "Error: Error page is not a readable file: ./var"},
	{"file": "98_error_page_in_location.conf", "exp": "Error: Server directive in wrong context: error_page"},
	{"file": "100_methods_empty.conf", "exp": "Error: allowed_methods needs at least one method"},
	{"file": "101_methods_lowercase.conf", "exp": "Error: Invalid HTTP method: get"},
	{"file": "102_methods_unknown.conf", "exp": "Error: Invalid HTTP method: PUT"},
	{"file": "103_methods_duplicate_value.conf", "exp": "Error: Duplicated methods: GET"},
	{"file": "104_methods_duplicate_directive.conf", "exp": "Error: Duplicate directive in location: allowed_methods"},
	{"file": "105_methods_in_server.conf", "exp": "Error: Route directive in wrong context: allowed_methods"},
	{"file": "106_autoindex_uppercase.conf", "exp": "Error: autoindex must be on or off"},
	{"file": "107_autoindex_bool.conf", "exp": "Error: autoindex must be on or off"},
	{"file": "108_upload_enabled_bad.conf", "exp": "Error: upload_enabled must be on or off"},
	{"file": "109_upload_no_store.conf", "exp": None},
	{"file": "110_upload_store_missing_dir.conf", "exp": "Error: upload_store is not a directory: ./var/nope/nope"},
	{"file": "111_root_missing_dir.conf", "exp": "Error: root is not a directory: ./does/not/exist"},
	{"file": "112_root_is_file.conf", "exp": "Error: root is not a directory: ./Makefile"},
	{"file": "113_root_duplicate.conf", "exp": "Error: Duplicate directive in location: root"},
	{"file": "114_root_two_values.conf", "exp": "Error: Invalid number of arguments in directive: root"},
	{"file": "115_root_in_server.conf", "exp": "Error: Route directive in wrong context: root"},
	{"file": "116_no_root_no_redirect.conf", "exp": "Error: Location needs root, alias or redirect: /"},
	{"file": "117_root_and_alias.conf", "exp": "Error: root and alias are exclusive in location: /"},
	{"file": "118_redirect_empty.conf", "exp": "Error: Invalid number of arguments in directive: redirect"},
	{"file": "119_redirect_self_loop.conf", "exp": None},
	{"file": "120_redirect_with_code.conf", "exp": "Error: Invalid number of arguments in directive: redirect"},
	{"file": "121_index_two_values.conf", "exp": "Error: Invalid number of arguments in directive: index"},
	{"file": "122_location_cmbs_negative.conf", "exp": "Error: Invalid client_max_body_size"},
	{"file": "123_location_path_traversal.conf", "exp": None},
	{"file": "130_cgi_no_dot.conf", "exp": "Error: CGI extension must start with '.'"},
	{"file": "131_cgi_dot_only.conf", "exp": "Error: Invalid CGI extension: ."},
	{"file": "132_cgi_missing_interpreter.conf", "exp": "Error: Invalid number of arguments in directive: cgi"},
	{"file": "133_cgi_interpreter_not_found.conf", "exp": "Error: CGI interpreter not executable: /usr/bin/doesnotexist"},
	{"file": "134_cgi_interpreter_not_exec.conf", "exp": "Error: CGI interpreter not executable: ./Makefile"},
	{"file": "135_cgi_duplicate_ext.conf", "exp": "Error: Duplicate CGI extension: .py"},
	{"file": "136_cgi_in_server.conf", "exp": "Error: Route directive in wrong context: cgi"},
	{"file": "137_cgi_three_args.conf", "exp": "Error: Invalid number of arguments in directive: cgi"},
	{"file": "140_server_name_duplicate.conf", "exp": "Error: Duplicate directive: server_name"},
	{"file": "141_server_name_empty.conf", "exp": "Error: Invalid number of arguments in directive: server_name"},
	{"file": "142_host_bad.conf", "exp": "Error: getaddrinfo: Name or service not known"},
	{"file": "143_host_in_location.conf", "exp": "Error: Server directive in wrong context: host"},
	{"file": "150_stress_5000_locations.conf", "exp": None},
	{"file": "151_stress_1000_servers.conf", "exp": None},
	{"file": "160_directory.conf", "exp": "Error: Config file is not a regular file"},
	{"file": "161_dev_urandom.conf", "exp": "Error: Config file is not a regular file"},
	{"file": "162_dev_zero.conf", "exp": "Error: Config file is not a regular file"},
	{"file": "163_broken_symlink.conf", "exp": "Error: Config file can not be opened"},
]

def run_configs(t):
	try:
		result = subprocess.run(
			["./webserv", "./tests/configsFiles/" + t["file"]],
			capture_output=True, text=True, errors="replace", timeout=2
		)
	except subprocess.TimeoutExpired:
		assert t["exp"] is None, "server started but an error was expected"
		return
	assert t["exp"] is not None, "server exited: " + result.stderr
	assert t["exp"] in result.stderr

@pytest.mark.parametrize("t", _requests, ids=lambda t: f'{t["file"]} {t["exp"]}')
def test_configs(t):
	run_configs(t)

def test_siege():
	result = subprocess.run(
		["siege", "-c", "20", "-r", "50", BASE_URL + "/"],
		capture_output=True, text=True
	)
	assert 0 == json.loads(result.stdout)["failed_transactions"]

def test_42():
	result = subprocess.run(
		["./tests/42tester/tester", "http://localhost:8888"],
		capture_output=True, text=True
	)
	assert "ERROR ON LAST TEST" not in result.stdout
