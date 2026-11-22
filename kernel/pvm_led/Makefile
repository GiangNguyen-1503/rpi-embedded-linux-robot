# Out-of-tree kernel module. Build ON the Pi (needs kernel headers):
#   make          -> pvm_led.ko
#   make clean
obj-m += pvm_led.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
