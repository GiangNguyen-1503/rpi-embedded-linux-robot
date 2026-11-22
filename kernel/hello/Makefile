# Out-of-tree kernel module. Build ON the Pi (needs kernel headers):
#   make          -> hello.ko
#   make clean
obj-m += hello.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
