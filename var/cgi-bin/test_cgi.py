
import sys, os

print("Content-Type: text/plain\r\n\r\n", end="")

for k in os.environ:
    print("<p><b>", k, "</b>=", os.environ.get(k, ""), "<p>")

data = sys.stdin.read()
print(f"{data}", end="")
