import os
import sys

from cgi import FieldStorage
from io import BytesIO as IO


# arguments = sys.argv[1:]
# print(f"\narguments: {arguments}\n")
print(f"\nos.environ: {os.environ}\n")

form = FieldStorage(
    IO(sys.stdin.read()),
    headers={'content-type': 'multipart/form-data'},
    environ={"REQUEST_METHOD": "POST"}
)
print(f"\nform: {form}\n")
