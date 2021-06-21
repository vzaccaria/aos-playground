FROM ubuntu:18.04

RUN echo "rebuild v2"
RUN apt-get update
RUN apt-get install -y autoconf automake autotools-dev curl \
    libmpc-dev libmpfr-dev libgmp-dev gawk build-essential \
    bison flex texinfo gperf libtool patchutils bc zlib1g-dev tmux git \
    libexpat-dev
WORKDIR /root
RUN git clone https://github.com/riscv/riscv-gnu-toolchain.git
WORKDIR /root/riscv-gnu-toolchain
RUN git submodule update --init --recursive
RUN ./configure --prefix=/opt/riscv/toolchain
RUN make
RUN make install
RUN apt-get install -y python3 python3-pip ninja-build pkg-config libglib2.0-dev libpixman-1-dev
WORKDIR /root/riscv-gnu-toolchain/qemu
RUN ./configure --target-list=riscv64-softmmu
RUN make -j4
RUN make install
RUN apt-get install -y vim



