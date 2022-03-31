import sys
import os
import subprocess

tools = dict()
tools.update({'dprofile':
             {'cache':'--tool=cachegrind',
              'callgraph':'--tool=callgrind'}
             })

if len(sys.argv)<4:
  print("not enough arguments")
  sys.exit(1)
if sys.argv[1] not in tools.keys():
  print("wrong tool name")
  sys.exit(1)
tool = sys.argv[1]

if sys.argv[2] not in tools[tool].keys()
  print("wrong mode name")
    sys.exit(1)
mode = tools[tool][sys.argv[2]]

args = [tool, mode]
args += sys.argv[3:]

p = subprocess.Popen(args)
