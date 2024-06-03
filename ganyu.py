import os
import hashlib
from datetime import datetime
import requests
import time

def get_file_hash(path):
    return hashlib.md5(open(path,'rb').read()).hexdigest()

# http://localhost:3000/upsrc?name=New+Source&description=No+Description&type=PHYSICAL&root=.%2F
def create_physical_source(path):
    URL = "http://localhost:3000/upsrc"
            
    PARAMS = {
        'name': 'Physical source',
        'description': 'Automatically generated physical source',
        'type': 'PHYSICAL',
        'root': path
    }

    requests.get(url = URL, params = PARAMS)
    time.sleep(0.5)

    print("Successfully created physical source")
        
# http://localhost:3000/upfile?name=file&ext=.txt&size=1&hash=ffffffffffffffffffffffffffffffff&src=1&access=.%2F
def upload_physical_files(path):
    for root, dirs, files in os.walk(path):
        path = root.split(os.sep)
        # print((len(path) - 1) * '---', os.path.basename(root))
        for file in files:
            fileName = os.path.splitext(file)
            filePath = root + '\\' + file
            fileStats = os.stat(filePath) 

            hash = get_file_hash(filePath)

            URL = "http://localhost:3000/upfile"
            
            PARAMS = {
                'name': fileName[0],
                'ext': fileName[1],
                'size': fileStats.st_size,
                'hash': hash,
                'src': 1,
                'access': root[3:]
            }

            requests.get(url = URL, params = PARAMS)
            time.sleep(0.5)

            print("Successfully uploaded " + fileName[0] + " file")

create_physical_source(".\\Documents")
upload_physical_files(".\\Documents")