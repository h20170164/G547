#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include<asm/uaccess.h>

#define CHANNEL_SELECT _IOW(0,0,unsigned long)

static dev_t adc_module;
static struct cdev c_dev;
static struct class *cls;

static int channel;
static int flag;

static int my_open(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Open()\n");
    return 0;
}
static int my_close(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Close()\n");
    return 0;
}
static ssize_t my_read(struct file *f,char __user *buffer,size_t length,loff_t *offset)
{
    char val[2];
    get_random_bytes(val,2);
    printk(KERN_INFO "Read()\n");
    put_user(val[0],buffer);
    put_user(val[1],buffer+1);
    return 0;
}
static long my_ioctl(struct file *f,unsigned int cmd,unsigned long arg)
{
    printk(KERN_INFO "IOCTL()\n");
    switch(cmd)
    {
        case CHANNEL_SELECT:
        channel=arg;
    }
    printk(KERN_INFO "ADC Channel is %d\n",channel);
    return 0;
}
static struct file_operations fops=
{
    .owner=THIS_MODULE,
    .open=my_open,
    .release=my_close,
    .read=my_read,
    .unlocked_ioctl=my_ioctl
};
static int __init my_init(void)
{
    if(alloc_chrdev_region(&adc_module,0,1,"ADCDRIVER")<0)
    {
        flag=-1;
        goto RET;
    }
    cls=class_create(THIS_MODULE,"adcclass");
    if(cls==NULL)
    {
        unregister_chrdev_region(adc_module,1);
        flag=-1;
        goto RET;
    }
    if(device_create(cls,NULL,adc_module,NULL,"adcdriver")==NULL)
    {
        class_destroy(cls);
        unregister_chrdev_region(adc_module,1);
        flag=-1;
        goto RET;
    }
    cdev_init(&c_dev,&fops);
    if(cdev_add(&c_dev,adc_module,1)<0)
    {
        device_destroy(cls,adc_module);
        class_destroy(cls);
        unregister_chrdev_region(adc_module,1);
        flag=-1;
        goto RET;
    }
    printk(KERN_INFO "ADC driver registered! MAJOR: %d MINOR: %d\n",MAJOR(adc_module),MINOR(adc_module));
    flag=0;
    channel=0;/*Channel set to 0 by default*/
    RET:
    return flag;
}
static void __exit my_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cls,adc_module);
    class_destroy(cls);
    unregister_chrdev_region(adc_module,1);
    printk(KERN_INFO "ADC driver unregisterd!\n");
}
module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Himanshu Singh");
MODULE_DESCRIPTION("ADC Driver");
