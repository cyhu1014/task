import os
import sqlite3
import hashlib
def test_md5 ():
    f = open("../../seg/part0002","rb")
    m = hashlib.md5()
    file = f.read()
    file_size = len(file)
    string1 = str(file).encode("utf-8")
    m.update(string1)
    md5_hash = m.hexdigest()
    print(string1)
    print(file_size,md5_hash)


def segtomd5 (abs_path,database_name,table_name):
    con = sqlite3.connect(database_name)
    cur = con.cursor()
    # Create table
    cur.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='%s';"%table_name)
    
    if len(cur.fetchall()) == 0 :
        print("create_table")
        cur.execute('''CREATE TABLE %s
               (ID INTEGER PRIMARY KEY   AUTOINCREMENT,
                data_name text, 
                data_size int, 
                md5_hash text)'''%table_name) 
    m = hashlib.md5()
    seg_list = sorted(os.listdir(abs_path))
    for i in seg_list:
        if i == ".DS_Store":
            continue
        f_name = abs_path+i
        f = open(abs_path+i,"rb")
        file = f.read()
        file_size = len(file)
        string1 = str(file).encode("utf-8")
        m.update(string1)
        md5_hash = m.hexdigest()
        print(f_name,file_size,md5_hash)
        cur.execute("SELECT * FROM %s WHERE md5_hash = '%s';"%(table_name,md5_hash))
        if (len(cur.fetchall())==0):
            cur.execute("INSERT INTO %s (data_name,data_size,md5_hash)VALUES ('%s','%d','%s')"%(table_name,f_name,file_size,md5_hash))
            con.commit()
    con.close()
if __name__=='__main__':
    
    database_name = input('Database name?\n')
    table_name = input('Table name?\n')
    fromfilepath = input('Filepath to be md5hash?\n')
    # segtomd5(fromfilepath,database_name,table_name)

    
    