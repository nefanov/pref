import sys
import os
import subprocess
#default tools
tools = dict()
tools.update({'cmake':
              {'cmake-build':'cmake ./ && make'
              'make':'make'},
              'valgrind':
             {'cache':'--tool=cachegrind',
              'callgraph':'--tool=callgrind'}
             })

def run(argv, tools=tools):
  if len(argv)<3:
    print("not enough arguments")
    sys.exit(1)
  if argv[1] not in tools.keys():
    print("wrong tool name")
    sys.exit(1)
  tool = argv[1]

  if argv[2] not in tools[tool].keys()
    print("wrong mode name")
      sys.exit(1)
  mode = tools[tool][argv[2]]

  args = [tool, mode]
  if len(argv)>3:
    args += argv[3:]

  p = subprocess.Popen(args)

  return

if __name__ == "__main__":
  run(sys.argv, tools=tools)
