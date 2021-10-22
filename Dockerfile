FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /home

RUN --mount=type=cache,id=build-cache,target=/var/cache/apt apt update && \
    apt install -y libblas-dev

COPY ./build/matrix-mul/matrix-mul ./

CMD ./matrix-mul