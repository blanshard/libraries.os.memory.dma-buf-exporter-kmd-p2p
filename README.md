# DMA Buffer Exporter Kernel Mode Driver

## Features

 * Creates a new character device /dev/dma_buf_exporter
 * Provides IOCTL interfaces DMA_BUF_EXPORTER_ALLOC and DMA_BUF_EXPORTER_FREE to allocate and free dma_buf
 * Exports interfaces to map and unmap dma_buf in kernel mode

## Prerequisites

* make
* gcc
* kernel headers

## Build, install load

```shell
sudo make all        # builds, installs, cleans and loads module.
make build           # builds module
sudo make install    # installs module into the system and enable autorun.
sudo make uninstall  # uninstalls module from the system
sudo make load       # loads module into kernel
sudo make unload     # unloads module from kernel
make clean           # cleans build files
```
To check module version number:
```shell
modinfo dma_buf_exporter_intel
```

After installation you can use **modprobe** and other Linux's tool for modules.


## Build flags

* -Wall:   Enables all compilers warning messages
