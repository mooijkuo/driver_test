#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <fcntl.h>
#include <linux/watchdog.h> 
#include <errno.h>
#include <unistd.h>

int main(int argc, const char *argv[]) 
{
	printf("\n======== wdt test ========\n");
	
	int fd;
	printf("\n open watchdog \n");
	fd = open("/dev/watchdog",O_RDWR);
	if(fd == -1){
		perror("watchdog");
		return -1;
	}

	printf("\n set timeout 10 seconds.\n");
	int timeout = 10,ret;
	ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
	if(ret < 0){
		perror("ioctl");
		return -1;
	}
	printf("\n start wdt \n");
	write(fd, "\0", 1);

	int i=0;
	while(i < 5){
		sleep(5);
		i++;
		printf("\n 5 seconds send wdt KEEPALIVE! %d times. \n",i);
		ioctl(fd, WDIOC_KEEPALIVE, 0);
	}
	printf("\n Please wait 10 seconds to reset! \n");

	printf("\n close watchdog\n");
	close(fd);
	printf("\n====== wdt test end ======\n");
	return 0;
}