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

print(f'<div class="containers">')
for k, v in jsondata.items():
	if (k != 'routes'):
		print(f'<div class="container">')
		print(f'<p>{html.escape(str(k))}: {html.escape(str(v))}</p>')
		print(f'</div>')
print(f'</div>')

# ########################
