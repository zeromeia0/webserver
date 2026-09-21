from common.footer import footer
from common.header import header
import json, html

header('ROUTES')

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
for i in jsondata['routes']:
	print(f'<div class="container">')
	for k, v in i.items():
		print(f'<div class="item">')
		print(f'<p>{html.escape(str(k))}: {html.escape(str(v))}</p>')
		print(f'</div>')
	print(f'</div>')
print(f'</div>')

# ########################

footer()