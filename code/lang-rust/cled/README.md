
# Install [Platformio](https://platformio.org/) and compile the example C blinking app

```shell
# you might need to install python3.10-venv
wget -O get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py

cd cled
make firmware.elf

renode ../renode-script.sh
```
