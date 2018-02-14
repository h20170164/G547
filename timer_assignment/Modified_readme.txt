Delay is implemented using 8254 PIT

to run-

1.
make all
sudo insmod timer1.ko

This will load the device driver into kernel.

2.
$ gcc -o asm timer_user.c
$ sudo ./asm r /*to read*/
$ sudo ./asm s /*to sleep*/


PROBLEM-device creation(timer_device)
