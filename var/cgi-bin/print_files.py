import os

dirs = os.listdir(path='./var/uploads/')

print(f'<button>')
print(f'<div>ADD</div>')
print(f'</button>')

print(f'<div class="containers">')
for i in dirs:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	print(f'		<p>{i}</p>')
	print(f'<button>')
	print(f'<div>DELETE</div>')
	print(f'</button>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')
