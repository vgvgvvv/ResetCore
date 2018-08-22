# coding=utf-8
import json
import os
import sys

print(sys.path[0])
print(os.path.abspath(os.curdir))

file = open("test/sys_env.json", "r")
data = json.load(file)

print(data["MoonABPath"])