#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>             /*file operations*/ 
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/io.h>                /*or outb*/
#include <linux/uaccess.h>      /*put_usr/copy_to_usr*/

#define SLEEP _IOW(0,0,unsigned long)

static dev_t timer;// allocates maj and minr
static struct cdev timer_dev;//for character device structure
static struct class *timer_cls;// for device class

unsigned int a=1193;
unsigned int flag1,flag2;

static int timer_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Timer opened\n");
    return 0;
}
 
static int timer_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Timer closed\n");
    return 0;
}
 
static ssize_t timer_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 unsigned int val_timer = inb(0x42); //port 0x40 read (lower byte value)
 put_user (val_timer , buf);
 val_timer = inb (0x42);            // port 0x40 read (higher byte value)
 put_user (val_timer , buf+1);
 
    printk(KERN_INFO "Timer read\n");
    return 0;
}

void delayms(void)
{
outb_p(0xB6,0x43);	  //square wave generator,channel 2
outb_p(a&0xff,0x40);
outb_p(a>>8,0x40);
flag1=inb(0x61);  
flag1=(flag1)&(0x20);
}

void sleep(unsigned long val)
{
while(val!=0)
{
flag2=(inb(0x61)&(0x20));
if(flag2!=flag1)
{
(val--);
flag1=flag2;
}
}
} 
static long timer_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "IOCTL()");
     switch(cmd)
	{
	case SLEEP: sleep(arg);
		break;	 
	}
return 0;
}


static struct file_operations fops =
{
  .owner    = THIS_MODULE,
  .open     = timer_open,
  .release  = timer_close,
  .read     = timer_read,
  .unlocked_ioctl=timer_ioctl
};


static int __init timer_init(void) 
{
    printk(KERN_INFO "TIMER Registered");
     
    
    if (alloc_chrdev_region(&timer, 0, 1, "TIMER_DEVICE") < 0)   //dynamic allocation
    {
        return -1;
    }
     
    
    if ((timer_cls = class_create(THIS_MODULE, "chardrv")) == NULL)//create device node in /dev directory
    {
        unregister_chrdev_region(timer, 1);
        return -1;
    }
    if (device_create(timer_cls, NULL, timer, NULL, "timer_device") == NULL) // timer_device name in dev directory
    {
        class_destroy(timer_cls);
        unregister_chrdev_region(timer, 1);
        return -1;
    }
     
    // Link fops and cdev to device node

    cdev_init(&timer_dev, &fops);
    if (cdev_add(&timer_dev, timer, 1) == -1)
    {
        device_destroy(timer_cls, timer);
        class_destroy(timer_cls);
        unregister_chrdev_region(timer, 1);
        return -1;
    }
    return 0;
}





static void __exit timer_exit(void) 
{
    cdev_del(&timer_dev);
    device_destroy(timer_cls,timer);
    class_destroy(timer_cls);
    unregister_chrdev_region(timer,1);
    printk(KERN_INFO "timer device unregistered\n\n");
}
  
module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("HIMANSHU SINGH");
MODULE_DESCRIPTION("TIMER");
