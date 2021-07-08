FROM ubuntu:groovy

RUN echo "rebuild"
RUN apt-get update
RUN apt-get install -y wget git qemu-system qemu-utils python3 python3-pip \
        gcc libelf-dev libssl-dev bc flex bison vim bzip2  cpio

RUN apt-get install -y parted
RUN apt-get install -y dosfstools
