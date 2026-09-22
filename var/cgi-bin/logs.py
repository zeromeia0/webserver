import os, json, ast, html, sys
from urllib.parse import unquote

# ########################
# FUNCTIONS
# ########################

# ########################
# LOGIC
# ########################

with open("var/data/logs.json") as f:
	data = f.read()
jsondata = json.loads(data)
entries = jsondata["entries"]

QUERY_STRING = unquote(os.getenv("QUERY_STRING", ""))

splits = QUERY_STRING.split("&")
qs = {}
for s in splits:
	tmp = s.split("=")
	if (len(tmp) == 2):
		qs[tmp[0]] = tmp[1]

for q in qs:
	qs[q] = ast.literal_eval(qs[q])

if (qs.get("status")):
	entries = list(filter(lambda d: d["status"] in qs["status"], entries))
if (qs.get("type")):
	entries = list(filter(lambda d: d["type"] in qs["type"], entries))
if (qs.get("method")):
	entries = list(filter(lambda d: d["method"] in qs["method"], entries))

entries = sorted(entries, key=lambda x: x["timestamp"], reverse=True)

print(f'<div class="containers">')
for log in entries:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	for k, v in log.items():
		print(f'<p>{html.escape(str(k))}: {html.escape(str(v))}</p>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')

# ########################
