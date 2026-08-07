import sys, json, os

body = sys.stdin.read()
data = json.loads(body)

print(f'<div>{data["name"]}</div>')
