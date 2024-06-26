#!/bin/bash

# build ganyu app
cmake -B build -G Ninja
cmake --build build

# copy it to backend image directory
cp ./build/src/ganyu -f ./docker/backend/ganyu

cd ./docker

# create a ganyu docker virtual network
docker network rm ganyu_network
docker network create -d bridge --subnet=172.22.2.0/24 ganyu_network

# compuse and run docker cluster
docker-compose -f docker-compose.yml up -d

cd ..