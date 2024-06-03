# ganyu
ganyu - file management utility program. 

### Overview
ganyu - file management utility program. Main idea of this project is just make some sort of global file index for all you files across diferent source, cloud storages such as **Google Drive**, **OneDrive**, etc. websites (for example image boards) and ofcourse your local physical storages. Ganyu allows creation of *Virtual* directories that can link to files across diferent sources.

Ganyu written internally with C (application itself), and uses PostgreSQL as backend database. Everything is packed into single **Docker** cluster, for fast and easy deployment.  

**Note that ganyu is not a proper maintained project, it was made just for study purposes***

> Ganyu is not a new javascript framework btw.

### Links
1. Source code avaiable at [github.com/Maksasj/ganyu](https://github.com/Maksasj/ganyu)
2. **chttp.h** and other C libraries source code [github.com/Maksasj/caifu](https://github.com/Maksasj/caifu)

## Build & Run

> Building was tested on **Ubuntu 22.04 jammy**

First of all requirements:
- Cmake (At least version 3.21)
- Ninja (At least version 1.11.1)
- C++ compiler (Have test with Clang 15.0.5 and GCC 12.2.0) 
- Docker

After you can follow this workflow for deploying **Docker** cluster
```bash
# build ganyu app
cmake -B build -G Ninja
cmake --build build

# copy it to backend image directory
cp ./build/src/ganyu -f ./docker/backend/ganyu

cd ./docker

# create a ganyu docker virtual network
docker network rm ganyu_network
docker network create -d bridge --subnet=172.22.2.0/24 ganyu_network

# compose and run docker cluster
docker-compose -f docker-compose.yml up -d

cd ..
```
or just use **start.sh** script.

## License
Ganyu is free and open program. All code in this repository is licensed under
- MIT License ([LICENSE.md](https://github.com/Maksasj/ganyu/blob/master/LICENSE.md) or https://opensource.org/license/mit/)
