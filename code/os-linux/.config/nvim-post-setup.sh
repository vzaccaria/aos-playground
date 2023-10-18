# Note this must be executed after building the container
# To save the state of the container after having executed this file
#
# docker ps << get the id of the machine 
# docker commit <container_id_or_name> lkp-dev 


apt-get update
apt-get install -y silversearcher-ag libncurses-dev tmux clang-format neovim curl 

cp /repo/.config/.tmux.conf /root
cp /repo/.config/.vimrc /root


# Install required version of node 

curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.34.0/install.sh | /bin/bash
export NVM_DIR="$HOME/.nvm" 
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
nvm install 14.14.0

sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'

apt-get install -y clangd
npm install coc-clangd 

apt remove neovim -y 
wget https://github.com/neovim/neovim/releases/download/stable/nvim-linux64.tar.gz
tar xzvf nvim-linux64.tar.gz -C /usr/local --strip-components=1

mkdir -p ~/.config/nvim
ln -sf ~/.vimrc ~/.config/nvim/init.vim

nvim --headless +PlugInstall +qall
nvim --headless +"CocInstall -sync coc-clangd" +qall

