import os

FOLDER = os.environ.get("FOLDER")
BASE_PATH = os.environ.get("BASE_PATH")

print(f'<h2>{FOLDER}</h2>')

for i in os.listdir(FOLDER):
	print(f'<div><a href="{BASE_PATH}/{i}" >{i}<a></div>')
