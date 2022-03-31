import sys
import os

tools = dict()
tools.update({'profile':
             {'cache':'cache',
              'callgraph':'callgraph'}
             })

if len(sys.argv)<2:
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

