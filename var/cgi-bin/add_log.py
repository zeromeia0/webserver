import json, os, time

with open("var/data/logs.json", "r") as f:
	data = f.read()

jsondata = eval(data)
BODY = json.loads(os.getenv("BODY"))
jsondata["entries"].append(BODY)

with open("var/data/logs.json", "w") as f:
	f.write(json.dumps(jsondata))
