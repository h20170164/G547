obj-m := adc_kernel.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
