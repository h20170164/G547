#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define SLEEP _IOW(0,0,unsigned long)

int main(int argc,char *argv[])
{
    int fd;
    int ret_val;
    char value[2];
    unsigned long sleep_val;
    fd=open("/dev/timer_device",0);
    if(fd<0)
    {
        printf("Timer Failed\n");
        exit(-1);
    }
    if(argc==2 && argv[1][0]=='r')
    {
        ret_val=read(fd,value,2);
	if(ret_val>=0)
	{
	    printf("Read MS %d",(unsigned char)value[0]+(unsigned char)value[1]*256);
	}
	else
	printf("Timer Read failed\n");
    }
    if(argc==2 && argv[1][0]=='s')
    {
        scanf("%lu",&sleep_val);   
        system("date"); 
        ret_val=ioctl(fd,SLEEP,sleep_val);
	system("date");
        if(ret_val>=0)
	{
        printf("IOCTL completed successfully");
	}        
	else
        printf("Timer IOCTL Failed\n");
    }
    close(fd);
return 0;
}
