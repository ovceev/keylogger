# Keylogger
## Keylogger for a test task

The task is to write a linux kernel module for keylogging. Next step is to get around the SElinux policy. 

## References

- https://medium.com/@knownsec404team/how-to-develop-linux-driver-from-scratch-cc143e0c08a1
- http://redgetan.cc/lets-write-a-kernel-keylogger/
- https://github.com/AlirezaChegini/kernel-based-keylogger-for-Linux
- https://github.com/jarun/spy
- https://elixir.bootlin.com/linux/latest/source/include/linux/debugfs.h
- https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-4.html
- https://habr.com/ru/post/322476/
- https://losst.ru/nastrojka-selinux
- https://0x00sec.org/t/linux-keylogger-and-notification-chains/4566
- https://defcon.ru/os-security/1264/
- https://github.com/hackerschoice/THC-Archive/blob/master/Papers/writing-linux-kernel-keylogger.txt
- http://dmilvdv.narod.ru/Translate/LDD3/Linux_Device_Drivers_3_ru.pdf

## Installation

Install the dependencies.
```sh
sudo apt update
sudo apt install build-essential
```

Build
```
make
```

Insert the kernel module
```
sudo insmod keylogger.ko
```

Key pressed are stored in ```/sys/kernel/debug/evidence```

See if the module is loaded
```
lsmod | grep keylogger
```

Info about the module
```
modinfo keylogger
```

Remove the module
```
sudo rmmod keylogger
make clean
```