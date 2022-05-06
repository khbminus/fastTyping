#!/usr/bin/env python3
import socket

s = socket.socket()
s.connect(('46.17.248.131', 1337))
f = s.makefile(mode='rw')

print("Logining...")
f.write('{"header": { "type": "hello"},"body": {"name": "servertest123"}}\n')
f.flush()
assert f.readline() == '{"body":{"name":"servertest123"},"header":{"queryType":"hello","type":"loginSuccessfully"}}\n'
f.write('{"header": { "type": "echo"},"body": {"name": "it\' working"}}\n')
f.flush()
assert f.readline() == '{"header":{"queryType":"echo"},"name":"it\' working"}\n'
print("All works!")
