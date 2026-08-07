with open("var/data/configs.json") as f:
	data = f.read()
jsondata = eval(data)

print(f'<div class="containers">')
for i in jsondata['routes']:
	print(f'<div class="container">')
	for k, v in i.items():
		print(f'<div class="item">')
		print(f'	<p>{k}: {v}</p>')
		print(f'</div>')
	print(f'</div>')
print(f'</div>')
