/* /home/mooij/test_i2c.c
*
* Copyright (c) 2010 SQ Tech. CO., LTD. <http://www.sq.com.tw/>
* mooijKuo <mooijKuo@sq.com.tw>
*
* USER layer - i2c
*
* THIS SOFTWARE IS PROVIDED UNDER LICENSE AND CONTAINS PROPRIETARY
* AND CONFIDENTIAL MATERIAL WHICH IS THE PROPERTY OF SQ TECH.
*
* Modifications:
* $Id$
*
*/

#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define I2C_RETRIES		0x0701
#define I2C_TIMEOUT		0x0702
#define I2C_I2C_SLAVE_FORCE	0x0706
#define I2C_RDWR		0x0707

#define I2C_M_TEN	0x0010
#define I2C_M_RD	0x0001


#define ADDRESS		ADV7179
#define E2PROM		0x50
#define ADV7179		0x2a

struct i2c_msg
{
	__u16 addr;
	__u16 flags;
	__u16 len;
	__u8 *buf;
};

struct i2c_rdwr_ioctl_data
{
	struct i2c_msg *msgs;
	int nmsgs;
};

struct i2c_arg
{
	char id;
	char addr;
	char val;
};

	int fd,ret;

char i2c_read_byte(char id,char addr)
{
	struct i2c_rdwr_ioctl_data alldata;
	char tmp=0;
	
	alldata.nmsgs = 2;
	alldata.msgs = (struct i2c_msg*)malloc(alldata.nmsgs * sizeof(struct i2c_msg));
	if(!alldata.msgs){
		printf("Memory alloc error!\n");
		close(fd);
	return 0;
	}

	(alldata.msgs[0]).addr   = id;                     //slave address
	(alldata.msgs[0]).flags  = 0;                      //flag = 0(write) , 1(read)
	(alldata.msgs[0]).len    = 1;
	(alldata.msgs[0]).buf    = (__u8 *)malloc(1);
	(alldata.msgs[0]).buf[0] = addr;                   //sub address (offset)
	(alldata.msgs[1]).addr   = id;
	(alldata.msgs[1]).flags  = I2C_M_RD;
	(alldata.msgs[1]).len    = 1;
	(alldata.msgs[1]).buf    = (__u8 *)malloc(1);
	(alldata.msgs[1]).buf[0] = 0;

	ret = ioctl(fd,I2C_RDWR,(unsigned long)&alldata);
	if(ret < 0)
		printf("R ER id = 0x%2x ,address = 0x%2x val=0x%2x ,ret=0x%x\n",id,addr,(alldata.msgs[1]).buf[0],ret);	
	else 
		printf("R OK id = 0x%2x ,address = 0x%2x val=0x%2x\n",id,addr,(alldata.msgs[1]).buf[0]);
	
	tmp = (alldata.msgs[1]).buf[0];
	free((alldata.msgs[0]).buf);
	free((alldata.msgs[1]).buf);
	free(alldata.msgs);
	
	return tmp;
}

int i2c_write_byte(char id,char addr,char val)
{
	struct i2c_rdwr_ioctl_data alldata;
	
	alldata.nmsgs = 1;
	alldata.msgs = (struct i2c_msg*)malloc(alldata.nmsgs * sizeof(struct i2c_msg));
	if(!alldata.msgs){
	printf("Memory alloc error!\n");
	close(fd);
	return 0;
	}

	(alldata.msgs[0]).addr   = id;
	(alldata.msgs[0]).flags  = 0;
	(alldata.msgs[0]).len    = 2;
	(alldata.msgs[0]).buf    = (__u8 *)malloc(2);
	(alldata.msgs[0]).buf[0] = addr;
	(alldata.msgs[0]).buf[1] = val;  //write value

	ret = ioctl(fd,I2C_RDWR,(unsigned long)&alldata);
	if(ret < 0)
		printf("W ER id = 0x%2x ,address = 0x%2x val=0x%2x ret=0x%4x\n",id,addr,val,ret);
	else
		printf("W OK id = 0x%2x ,address = 0x%2x val=0x%2x \n",id,addr,val);	
	
	free((alldata.msgs[0]).buf);
	free(alldata.msgs);
	
	return 0;
}

int main(int argc,char **argv)
{
	struct i2c_rdwr_ioctl_data alldata;
	char orgval,rdval=0,wrval=0x5a;

	printf("Open device : /dev/i2c-0\n");
	fd = open("/dev/i2c-0", O_RDWR);
	if(!fd){
	printf("Error on opening the device file!\n");
	return 0;
	}

	ioctl(fd,I2C_TIMEOUT,1);
	ioctl(fd,I2C_RETRIES,1);
	
	if(argc <2){
		orgval = i2c_read_byte(ADDRESS,0x00);
		printf("origin val = 0x%x\n",orgval);
	
		i2c_write_byte(ADDRESS,0x00,wrval);
		printf("write value = 0x%x\n",wrval);
	
		rdval = i2c_read_byte(ADDRESS,0x00);
		printf("read val = 0x%x\n",rdval);
	
		i2c_write_byte(ADDRESS,0x00,orgval);
		printf("write origin value 0x%x\n",orgval);
	
		rdval = i2c_read_byte(ADDRESS,0x00);
		printf("read val = 0x%x\n",rdval);
	}
	else{
		if(access(argv[1],F_OK) == 0){
			int filefd;
			struct i2c_arg argtmp = {0,0,0};
			filefd = open(argv[1],O_RDONLY);
			while(read(filefd,&argtmp,sizeof(argtmp)) != 0)
			{
				i2c_write_byte(argtmp.id,argtmp.addr,argtmp.val);
				i2c_read_byte(argtmp.id,argtmp.addr);
			}
		}
		else
			printf("%s does not found!!\n",argv[1]);
	}
	close(fd);
	return 0;
}