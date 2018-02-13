Delay is implemented using 8254 PIT

to run-

1.
$ make all
$ sudo insmod timer1.ko

This will load the device driver into kernel.

#2
$ gcc timer_user.c -o timer
$ sudo ./timer
