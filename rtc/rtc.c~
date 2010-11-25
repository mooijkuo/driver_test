#include <stdio.h> 
#include <linux/rtc.h> 
#include <sys/ioctl.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <errno.h>
#include <stdlib.h>

#define RTC_PWR_OFF		_IO('p', 0x23)

int main (int argc,char *argv[])
{
	printf("\n======== rtc test ========\n");
	
	int fd, retval,i;
	unsigned long freq,rdfreq=0;
	char test_item;
	struct rtc_time tm,rdtm;
	struct rtc_wkalrm alarm,rdalarm;
	
	fd = open ("/dev/rtc0", O_RDWR);
	if(fd == -1){
		perror("\nCan't open /dev/rtc0!\n");
		exit(errno); 
	}

	// Set periodic IRQ rate
	freq = 16;
	retval = ioctl(fd, RTC_IRQP_SET, freq); 
	if(retval == -1){ 
		perror("\nioctl RTC_IRQP_SET fail!\n");
		exit(errno); 
	}
	fprintf(stderr, "\nSet periodic IRQ rate is %ldHz\n", freq);

	// Read periodic IRQ rate
	retval = ioctl(fd, RTC_IRQP_READ, &rdfreq); 
	if (retval == -1) { 
		perror("\nioctl RTC_IRQP_READ fail!\n");
		exit(errno); 
	} 
	fprintf(stderr, "\nRead periodic IRQ rate is %ldHz\n", freq);
	
	// Set date/time: 2009-12-31 23:59:55 w(4)
	tm.tm_wday = 4;
	tm.tm_year = 2009 - 1900;
	tm.tm_mon = 12 - 1;
	tm.tm_mday = 31;
	tm.tm_hour = 23;
	tm.tm_min = 59;
	tm.tm_sec = 55;
	
	retval = ioctl(fd, RTC_SET_TIME, &tm); 
	if (retval == -1) { 
		perror("\nioctl RTC_SET_TIME fail!\n");
		exit(errno); 
	}
	fprintf(stderr, "\nSet RTC date/time is \n%04d-%02d-%02d, %02d:%02d:%02d w(%d)\n", tm.tm_year+1900,
				tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_wday);
	
	// Read date/time
	sleep(1);
	retval = ioctl(fd, RTC_RD_TIME, &rdtm); 
	if (retval == -1) { 
		perror("\nioctl RTC_RD_TIME fail!\n");
		exit(errno); 
	}
	printf("\nRead RTC date/time is \n%04d-%02d-%02d, %02d:%02d:%02d w(%d)\n", rdtm.tm_year+1900,
				rdtm.tm_mon+1, rdtm.tm_mday, rdtm.tm_hour, rdtm.tm_min, rdtm.tm_sec, rdtm.tm_wday);
	
	//Set alarm date/time: 2010-01-01 00:00:05 w(5)
	alarm.time.tm_wday = 5;
	alarm.time.tm_year = 2010 - 1900;
	alarm.time.tm_mon = 1 - 1;
	alarm.time.tm_mday = 1;
	alarm.time.tm_hour = 0;
	alarm.time.tm_min = 0;
	alarm.time.tm_sec = 5;
	
	retval = ioctl(fd, RTC_WKALM_SET, &alarm); 
	if (retval == -1) { 
		perror("\nioctl RTC_WKALM_SET fail!\n");
		exit(errno); 
	}
	fprintf(stderr, "\nSet RTC alarm date/time is \n%04d-%02d-%02d, %02d:%02d:%02d w(%d)\n", alarm.time.tm_year+1900, 
				alarm.time.tm_mon+1, alarm.time.tm_mday, alarm.time.tm_hour, alarm.time.tm_min,
				alarm.time.tm_sec, alarm.time.tm_wday);

	//Read alarm date/time
	sleep(1);
	retval = ioctl(fd, RTC_WKALM_RD, &rdalarm); 
	if (retval == -1) { 
		perror("\nioctl RTC_WKALM_RD fail!\n");
		exit(errno); 
	}
	printf("\nRead RTC alarm date/time is \n%04d-%02d-%02d, %02d:%02d:%02d w(%d)\n", rdalarm.time.tm_year+1900, 
				rdalarm.time.tm_mon+1, rdalarm.time.tm_mday, rdalarm.time.tm_hour, rdalarm.time.tm_min,
				rdalarm.time.tm_sec, rdalarm.time.tm_wday);

	//Wait for alarm
	fprintf(stderr, "\nWaiting 9 seconds for alarm...\n");
	for(i=0;i<9;i++){
		// Read date/time
		sleep(1);
		retval = ioctl(fd, RTC_RD_TIME, &rdtm); 
		if (retval == -1) { 
			perror("\nioctl RTC_RD_TIME fail!\n");
			exit(errno); 
		}
		printf("Read RTC date/time is %04d-%02d-%02d, %02d:%02d:%02d w(%d)\n", rdtm.tm_year+1900,
				rdtm.tm_mon+1, rdtm.tm_mday, rdtm.tm_hour, rdtm.tm_min, rdtm.tm_sec, rdtm.tm_wday);
	}
	
	close(fd);
	printf("\n====== rtc test end ======\n");
	return 0;
}