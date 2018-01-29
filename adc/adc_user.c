#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#define CHANNEL_SELECT _IOW(0,0,unsigned long)
int main(int argc,char *argv[])
{
    int fd;
    int flag;
    char *value;
    value=(char*)malloc(sizeof(char)*2);
    fd=open("/dev/adcdriver",0);
    if(fd<0)
    {
        printf("ADC Driver Open Failed\n");
        exit(-1);
    }
    if(argc==2 && (argv[1][0]=='R' || argv[1][0]=='r'))
    {
        flag=read(fd,value,2);
        if(flag>=0)
        printf("ADC Value: %d\n",((unsigned char)value[0])*4+((unsigned char)value[1])/64);
        else
        printf("ADC Driver Read Failed\n");
    }
    if(argc==2 && argv[1][0]>='0' && argv[1][0]<='7')
    {
        flag=ioctl(fd,CHANNEL_SELECT,argv[1][0]-48);
        if(flag>=0)
        printf("ADC Channel set to %d\n",argv[1][0]-48);
        else
        printf("ADC Driver IOCTL Failed\n");
    }
    close(fd);
    return 0;
}
