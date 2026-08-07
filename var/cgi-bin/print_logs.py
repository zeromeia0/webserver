with open("var/data/logs.json") as f:
	data = f.read()
jsondata = eval(data)

print(f'<div class="containers">')
for log in jsondata["entries"]:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	for k, v in log.items():
		print(f'	<p>{k}: {v}</p>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')
