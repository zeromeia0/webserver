import json, os, time

with open("var/data/logs.json", "r") as f:
	data = f.read()

jsondata = eval(data)
BODY = json.loads(os.getenv("BODY"))
BODY["timestamp"] = time.time()
jsondata["entries"].append(BODY)
sorted(jsondata["entries"], key=lambda item: item["timestamp"], reverse=True)
jsondata["entries"] = jsondata["entries"][:100]

with open("var/data/logs.json", "w") as f:
	f.write(json.dumps(jsondata))
