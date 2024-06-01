# Ganyu
ganyu - file management utility program. 
***Note that ganyu is not a proper maintained project, it was made just for study purposes***

> Ganyu is not a new javascript framework btw.

## Build & Run
Follow this workflow
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
