import json, html

# ########################
# FUNCTIONS
# ########################


# ########################
# LOGIC
# ########################

with open("var/data/configs.json") as f:
	data = f.read()
jsondata = json.loads(data)

print(f'<html>')
print(f'	<head>')
print(f'		<title>Webserv</title>')
print(f'		<meta charset="utf-8">')
print(f'		<link rel="stylesheet" href="./css/styles.css">')
print(f'	</head>')
print(f'<div class="containers">')
print(f'<div class="containers">')
for i in jsondata['routes']:
	print(f'<div class="container">')
	for k, v in i.items():
		print(f'<div class="item">')
		print(f'<p>{html.escape(str(k))}: {html.escape(str(v))}</p>')
		print(f'</div>')
	print(f'</div>')
print(f'</div>')
print(f'</html>')

# ########################
