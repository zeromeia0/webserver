from common.footer import footer
from common.header import header
import os, json

header('FILES')

# ########################
# FUNCTIONS
# ########################

print("""
<script>
async function deleteFile(file) {
	console.log(file)
	const url = "../uploads/"
	await fetch(url + file, {
		"method": "DELETE",
	})
	window.location = "./files.py"
}
</script>
""")
print("""
<script>
async function uploadFile() {
	const form = new FormData();
	const file = document.querySelector('input[type="file"]').files
	console.log(file[0])
	form.append('file', file[0])
	console.log(form)
	const url = "../uploads/" + file[0].name
	console.log(url)
	await fetch(url, {
		"method": "POST",
		"body": form,
	})
	window.location = "./files.py"
}
</script>
""")

# ########################
# LOGIC
# ########################

dirs = os.listdir(path='./var/uploads/')

print(f'<input type="file" name="file" accept="*/*" />')
print(f'<button onclick="uploadFile()">')
print(f'<div>ADD</div>')
print(f'</button>')

print(f'<div class="containers">')
for i in dirs:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	print(f'		<p>{i}</p>')
	print(f'<button onclick="deleteFile(\'{i}\')">')
	print(f'<div>DELETE</div>')
	print(f'</button>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')

# ########################

footer()