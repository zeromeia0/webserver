from common.footer import footer
from common.header import header
import os, json

header('LOGS')

# ########################
# FUNCTIONS
# ########################


# ########################
# LOGIC
# ########################

with open("var/data/configs.json") as f:
	data = f.read()
jsondata = eval(data)

print(f'<div class="containers">')
for k, v in jsondata.items():
	if (k != 'routes'):
		print(f'<div class="container">')
		print(f'	<p>{k}: {v}</p>')
		print(f'</div>')
print(f'</div>')

# ########################

footer()