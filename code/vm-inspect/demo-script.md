
# Description
This folder contains a few helper utilities and demo scripts for the inspection of a Linux OS virtual memory system. Currently it is based on a Ubuntu virtual machine based on [Canonical's Multipass](https://multipass.run/)

# Preliminaries 

## Mount a shared folder 

```
multipass mount $(pwd)/shared primary:/shared
```

# Demos

# Break a system with no swap
```shell
tmux
vmstat -wa -S m 1 # one shell, show in MB.

# check we dont have swap
free -h # we dont have swap
cat /proc/meminfo | grep Swap
swapon --show

# stress the system. We have 1GB and this will 
# trigger the OOM to find a process to kill 

stress --vm 2 --vm-bytes 256M --vm-hang 0 # still okay? 
stress --vm 4 --vm-bytes 256M --vm-hang 0 
```


# Try to break it with swap
```shell
# Create a swapfile 
sudo dd if=/dev/zero of=/swapfile bs=1MB count=1024 # 1GB swap
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
swapon --show
stress --vm 4 --vm-bytes 256M --vm-hang 0 # still good
stress --vm 8 --vm-bytes 256M --vm-hang 0 # let us break the bank
```


