obj-m += memory_module.o
KERNELDIR ?= /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -fr *.markers *.order

