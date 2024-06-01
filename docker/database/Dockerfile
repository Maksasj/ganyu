FROM ubuntu/postgres:14-22.04_beta

ENV POSTGRES_PASSWORD=ganyu
ENV POSTGRES_USERNAME=ganyu
ENV POSTGRES_DB=ganyu

COPY docker-entrypoint-initdb.d /docker-entrypoint-initdb.d

RUN mkdir -p /home/ganyu
WORKDIR /home/ganyu
COPY . /home/ganyu

EXPOSE 5432