
import sys

print("Content-Type: text/plain\r\n\r\n", end="")

data = sys.stdin.read()

print(f"{data}", end="")
