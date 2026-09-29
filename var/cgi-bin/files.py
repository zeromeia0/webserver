import os, json, html

print("Content-Type: text/html; charset=utf-8\r\n\r\n", end="")

# ########################
# FUNCTIONS
# ########################

dirs = os.listdir(path='../uploads/')

print("<html>")

print("""
	<script>
	  	async function deleteFile(file) {
			const url = "../uploads/"
			await fetch(url + encodeURIComponent(file), {
				"method": "DELETE",
			})
			window.location = "./files.py"
		}

	async function uploadFile() {
		const form = new FormData();
		const file = document.querySelector('input[type="file"]').files
		form.append('file', file[0])
		const url = "../uploads/" + encodeURIComponent(file[0].name)
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

print(f'<input type="file" name="file" accept="*/*" />')
print(f'<button onclick="uploadFile()">')
print(f'<div>ADD</div>')
print(f'</button>')

print(f'<div class="containers">')
for i in dirs:
	print(f'<div class="container">')
	print(f'	<div class="item">')
	print(f'        <p>{html.escape(i)}</p>')
	print(f'<button data-name="{html.escape(i)}" onclick="deleteFile(this.dataset.name)">')
	print(f'<div>DELETE</div>')
	print(f'</button>')
	print(f'	</div>')
	print(f'</div>')
print(f'</div>')

# ########################

print("</html>")
