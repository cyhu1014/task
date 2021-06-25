import os
from md5 import MD5
import sqlite3
con = sqlite3.connect('example.db')
cur = con.cursor()
# Create table
cur.execute('''CREATE TABLE chunk
               (data_name text, data_size int, md5_hash text)''')

abs_path = "seg/"
seg_list = sorted(os.listdir(abs_path))

for i in seg_list:
    if i == ".DS_Store":
        continue
    f_name = abs_path+i
    f = open(abs_path+i,"rb")
    file = f.read()
    file_size = len(file)
    string1 = str(file)
    md5_hash = MD5.hash(string1)
    print(f_name,file_size,md5_hash)
    cur.execute("INSERT INTO chunk VALUES ('%s','%d','%s')"%(f_name,file_size,md5_hash))
    con.commit()

