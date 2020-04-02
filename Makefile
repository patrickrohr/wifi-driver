obj-m += wifi_example.o
# wifi_example-objs := main.o
ccflags-y += -g -DDEBUG -std=gnu99 -Wno-declaration-after-statement

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

