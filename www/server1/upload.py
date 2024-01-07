# !/usr/bin/env python

import os
import sys

from cgi import FieldStorage


SERVER_WRITE_PATH = os.environ['SERVER_WRITE_PATH']
form = FieldStorage(sys.stdin.buffer)

i = 0
for form_file in form.list or []:
    with open(f"{SERVER_WRITE_PATH}{form_file.filename}", 'wb') as fd:
        fd.write(form.getvalue(form_file.name)[i])
        i+=1

print("<h1>haha!</h1>")

# import time

# while True:
#     time.sleep(1)