#!/usr/bin/env python3
import json
import socket

s = socket.socket()
s.connect(('localhost', 1337))
f = s.makefile(mode='rw')


def login():
    login_query = {"header": {"type": "login"}, "body": {"name": "serverTest123", "password": "aboba123"}}
    login_json = json.dumps(login_query)
    f.write(login_json + '\n')
    f.flush()
    response = json.loads(f.readline())
    r_type = response['header']['type']
    r_id = response['body']['id']
    if r_type == 'success':
        assert r_id != -1
        print(f"Successfully logged in! id={r_id}")
    else:
        assert r_id == -1
        print(f"Error. type={r_type}")
        exit(1)


def register():
    register_query = {"header": {"type": "register"}, "body": {"name": "serverTest123", "password": "aboba123"}}
    register_json = json.dumps(register_query)
    f.write(register_json + '\n')
    f.flush()
    line = f.readline().rstrip()
    response = json.loads(line)
    if response["header"]["type"] == "success":
        assert response['body']['id'] != -1
        print(f"Successfully registered. id={response['body']['id']}")
    elif response["header"]["type"] == "nameAlreadyExists":
        assert response['body']['id'] == -1
        print(f"Name exists, loging in.... (id={response['body']['id']})")
        login()
    else:
        print(f"Unknown exception. Type={response['header']['type']}. Body={response['body']}")
        exit(1)


def echo():
    echo_query = {"header": {"type": "echo"}, "body": {"kek": "lol"}}
    echo_json = json.dumps(echo_query)
    f.write(echo_json + '\n')
    f.write(echo_json + '\n')
    f.flush()
    response: dict = json.loads(f.readline())
    response.pop("header")
    assert response == echo_query['body']


print("Trying to register...")
register()
print("Testing echo...")
echo()
print("All works!")
