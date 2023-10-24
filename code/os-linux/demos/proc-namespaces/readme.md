
# Introduction

This demonstration is divided into two parts. The first part, conducted on a fully configured Ubuntu, aims to provide a comprehensive overview of PID namespaces. 

The second part delves into the actual internal kernel structures that support PID namespaces, which is a more advanced topic.

The first part is based on this [blog post](https://blog.quarkslab.com/digging-into-linux-namespaces-part-1.html).

## Inspection of namespaces at the filesystem level

Recall that:

- The PID namespace enables the creation of nested process trees, allowing non-systemd processes to perceive themselves as root processes within a subtree, gaining relative IDs. 
- Processes may have multiple IDs due to their presence in several namespaces, but each namespace allows only one unique PID per process. 
- The relations between processes remain intact in the root namespace but are invisible within a nested one. Consequently, a process in a nested namespace cannot interact with its parent or any other process in an upper namespace due to its perceived PID as 1.

Open tmux and two panes in it (pane 1 and pane 2). In pane 1 to inspect current available namespaces, (should be only one, i.e., the "root" one) do
```
lsns | grep pid
```

To create a new PID namespace you use the `unshare --pid` command. You must specify the process will acquire the new "init"/"systemd" property of having PID=1 (`--fork /bin/bash`). Also, you must specify that the new process must have only a local-view of the PIDs (not the "root" one) which is typically done through the `--mount-proc` file system.

```
sudo unshare --pid --fork --mount-proc /bin/bash
```

If you try then to do a ps:

```
root@primary:/home/ubuntu# ps -fe
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  0 09:59 pts/3    00:00:00 /bin/bash
root           8       1  0 09:59 pts/3    00:00:00 ps -fe

```

If go to pane 2, in the root namespace, you will find that it `/bin/bash` has a different PID:

```shell
ps -fe | grep bash
```
In fact, in the root name space you can shows all the different PIDs that the process has at this moment on the lower process hierarchies

```
cat /proc/<rootPIDofbash>/status | grep NSpid
```

If you do the same in pane 1, you will find that it has only PID=1. Since the process cannot find something it cannot see, it is effectively isolated.

##  Internal kernel structures that support PID namespaces
When a task is created, it gets always a root pid. However, if it has been created in a newnamespace, it also gets additional pids stored in an array of `upid` (`pid→numbers`). These are accessible by visiting the namespace hierarchy and accessing the numbers array through `pid_nr_ns`. The accompanying module just prints all the pid numbers (root and otherwise) of all running processes. Here is a picture of the involved structures. 

![](Images/readme%202023-10-24%2010.26.24.excalidraw.png)
%%[🖋 Edit in Excalidraw](Images/readme%202023-10-24%2010.26.24.excalidraw.md), and the [dark exported image](Images/readme%202023-10-24%2010.26.24.excalidraw.dark.png)%%