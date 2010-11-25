#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


struct i2c_arg
{
	char id;
	char addr;
	char val;
};

int main()
{
// a struct


	int fd,i,ret;
// arg set { id, addr, value }
	struct i2c_arg data[] = {
		{0x2a,0x0,0x00},
		{0x2a,0x4,0x00},
		{0x2a,0x0,0x05},
		{0x2a,0x4,0x05},
		{0x2a,0x0,0x50},
		{0x2a,0x4,0x50},
		{0x2a,0x0,0xaa},
		{0x2a,0x4,0xaa},
		{0x2a,0x0,0xff},
		{0x2a,0x4,0xff},
	};
	
	fd = open("./adv7179",O_WRONLY | O_CREAT, S_IRWXU);
	for(i=0;i< sizeof(data)/sizeof(data[0]);i++){
		write(fd,&data[i],sizeof(data[i]));
		printf("write data[%d] : id = 0x%x, addr = 0x%x, val = 0x%x\n",i,data[i].id,data[i].addr,data[i].val);
	}
	close(fd);

	struct i2c_arg tmp = {0,0,0};
	fd = open("./adv7179",O_RDONLY);
	while(ret = read(fd,&tmp,sizeof(tmp)) != 0)
		printf("read data : id = 0x%x, addr = 0x%x, val = 0x%x\n",tmp.id,tmp.addr,tmp.val);
	close(fd);

/* one by one
	char id = 0x2a, addr = 0x0, val = 0x5a;

	fd = open("./data",O_WRONLY | O_CREAT, S_IRWXU);
	write(fd,&id,sizeof(id));
	write(fd,&addr,sizeof(addr));
	write(fd,&val,sizeof(val));
	close(fd);

	char tmp = 0x0;
	fd = open("./data",O_RDONLY);
	read(fd,&tmp,sizeof(tmp));
	printf("id = %x\n",tmp);
	read(fd,&tmp,sizeof(tmp));
	printf("addr = %x\n",tmp);
	read(fd,&tmp,sizeof(tmp));
	printf("val = %x\n",tmp);
	close(fd);
*/
	return 0;
}