import os

# file size in GB
file_size = 10
# one block is 1MB
BLOCK_SIZE=1048576

file_name = str(file_size) + "G.file"
fw = open(file_name, 'a')

char_list = []
for i in range(BLOCK_SIZE):
    char_list.append('a')

for i in range(file_size * 1024):
    for ch in char_list:
        fw.write(ch);
    fw.write('\n')
