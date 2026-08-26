from common.footer import footer
from common.header import header
import os, json

header('LOGS')

# ########################
# FUNCTIONS
# ########################

def filters():
	data = {
		"method": ["GET", "POST", "DELETE", "HEAD"],
		"status": ["200", "201", "301", "403", "404", "405", "408", "413", "500", "504"],
		"type": ["RES", "REQ"],
	}

	for k, v in data.items():
		print(f"<div>")
		for i in v:
			print(f"	<input name='{k}' value='{i}' type='checkbox'/>")
			print(f"	<label>{i}<label/>")
		print(f"</div>")
	print("""
	<button onclick="applyFilters()">Filter</button>
	<script>
	function applyFilters() {
		var uri = '?'
		console.log(uri)
		var filters = {};
		var checked = document.querySelectorAll('input[type=checkbox]:checked');
		for (var i = 0; i < checked.length; i++) { 
			var name = checked[i].name;
			var value = checked[i].value;
			if (!filters[name]) filters[name] = [];
			filters[name].push(value);
		}
		console.log(filters)
		for (const [key, value] of Object.entries(filters)) {
			console.log(key)
			uri = uri + key + '=['
			value.forEach((e) => {
				console.log(e)
				uri = uri + "'" + e + "',"
			})
			uri = uri + ']&'
		}
		console.log(uri)
		window.location = uri
	}
	</script>
	""")

# ########################
# LOGIC
# ########################

with open("var/data/logs.json") as f:
	data = f.read()
jsondata = json.loads(data)
entries = jsondata["entries"]

QUERY_STRING = os.getenv("QUERY_STRING", "\{\}")
qs = json.loads(QUERY_STRING)
for q in qs:
	qs[q] = eval(qs[q])
if (qs.get("status")):
	entries = list(filter(lambda d: d["status"] in qs["status"], entries))
if (qs.get("type")):
	entries = list(filter(lambda d: d["type"] in qs["type"], entries))
if (qs.get("method")):
	entries = list(filter(lambda d: d["method"] in qs["method"], entries))

entries = sorted(entries, key=lambda x: x["timestamp"], reverse=True)

filters()

print(f'<div class="containers">')
for log in entries:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	for k, v in log.items():
		print(f'	<p>{k}: {v}</p>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')

# ########################

footer()
