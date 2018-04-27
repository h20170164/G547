#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>

//#define EVENT_BUF_NUM 64

int main(int argc , char ** argv){
	int fd = -1;
	int i,version;
	size_t read_bytes;
	struct input_event event_buf;

	printf("Opening ...\n");

	fd = open("/dev/input/event5", O_RDONLY);
	if(fd < 0) {
		printf("\nUnable to read from the device\n");
		return(1);
	}

	printf("Reading ...\n");
	while(1) {
		read_bytes = read(fd, &event_buf, sizeof(event_buf));
		switch(event_buf.type) {
			case EV_KEY:
			switch(event_buf.value) {
				case 0:
				printf("Key Code: %d released\n", event_buf.code);
				if(event_buf.code == 31)
					printf("key s has been released\n");
				break;
				case 1:
				printf("Key Code: %d pressed\n", event_buf.code);
				if(event_buf.code == 31)
					printf("key s has been pressed\n");
				break;

				case 2:
				printf("Key pressed continue\n", event_buf.code);
				break;

				default:
				printf("UnKnown value: type %d, code %d, value %d\n",
				event_buf.type,
				event_buf.code,
				event_buf.value);
				break;
			}
			break;
			
		}
	} 
	close (fd);
}
