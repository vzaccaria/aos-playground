FROM ubuntu:focal
ARG DEBIAN_FRONTEND=noninteractive

RUN echo "rebuild n.4"
RUN apt-get update
RUN apt-get install -y wget git qemu-system qemu-utils python3 python3-pip \
        gcc libelf-dev libssl-dev bc flex bison vim bzip2  cpio

# Download kernel
RUN mkdir -p /sources
WORKDIR /sources
RUN wget https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/snapshot/linux-5.16-rc1.tar.gz
RUN tar xvzf linux-5.16-rc1.tar.gz
RUN mv linux-5.16-rc1 linux
RUN wget https://busybox.net/downloads/busybox-1.32.1.tar.bz2
RUN tar xvjf busybox-1.32.1.tar.bz2

# initial build, so as to speed up development
COPY ./scripts/build-k.sh /sources
RUN /sources/build-k.sh

WORKDIR /scripts
EXPOSE 5900
EXPOSE 6000

# Setting up neovim-based IDE
# Uncomment the following commands if you want to setup neovim 
# 
# RUN apt-get update
# RUN apt-get install -y silversearcher-ag libncurses-dev tmux clang-format neovim curl nodejs npm
# COPY .config/.tmux.conf /root
# COPY .config/.vimrc /root
# RUN sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
# RUN mkdir -p ~/.config/nvim
# RUN ln -s ~/.vimrc ~/.config/nvim/init.vim
# RUN nvim --headless +PlugInstall +qall
# RUN apt-get install -y clangd
# RUN npm install coc-clangd 
# RUN curl -sL https://deb.nodesource.com/setup_12.x | bash - 
# RUN apt -y install nodejs
# RUN nvim --headless +"CocInstall -sync coc-clangd" +qall

# Setting up a few tools for uefi demos
RUN apt-get install -y parted
RUN apt-get install -y dosfstools

