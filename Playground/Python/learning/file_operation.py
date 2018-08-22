import os
import sys

print(os.path.curdir)
print(os.pardir)

print(os.path.abspath(os.path.curdir))# 当前工作目录
print(os.path.abspath(os.pardir))# 当前工作目录的父目录

print(__file__)# 文件路径
print(sys.path[0])# 文件所在目录