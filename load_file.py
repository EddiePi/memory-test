import os

file_name_prefix = "1G_"
file_name_sufix = ".file"

def load(name):
  fr = open(file_name, "r")
  while 1:
    line = fr.readline()
    if not line:
      break
  

for i in range(0, 2):
  file_name = file_name_prefix + str(i) + file_name_sufix
  load(file_name)

print("press enter to end")
line = raw_input()
