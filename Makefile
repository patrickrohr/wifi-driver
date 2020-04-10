obj-m += wifi_driver.o
wifi_driver-objs := device.o config.o netdev.o wiphy.o
ccflags-y += -g -DDEBUG -std=gnu99 -Wno-declaration-after-statement

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
