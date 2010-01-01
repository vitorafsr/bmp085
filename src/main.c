#include <stdio.h>
#include <unistd.h>
#include <bmp085/bmp085_bst.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

int f;

//device_addr, register_addr, register_data, write_length
char bus_read(unsigned char addr, unsigned char reg_addr, unsigned char *data, unsigned char length)
{
	int ret = i2c_smbus_read_i2c_block_data(f, reg_addr, length, data);
	if (ret == -1)
	{
		perror("read");
		return -1;
	}

	return ret;
}

void delay(unsigned int usec)
{
	usleep(1000*usec);
}

//device_addr, register_addr, register_data, write_length
char bus_write(unsigned char addr, unsigned char reg_addr, unsigned char *data, unsigned char length)
{
	int ret = i2c_smbus_write_i2c_block_data(f, reg_addr, length, data);
	if (ret == -1)
	{
		perror("write");
		return -1;
	}

	return ret;
}

void init(int addr)
{
	f = open("/dev/i2c-2", O_RDWR);
	if (f < 0)
	{
		perror("open");
		 _exit(-1);
	}

	int ret = ioctl(f, I2C_SLAVE, addr);
	if (ret == -1)
	{
		perror("ioctl");
		_exit(-1);
	}
}

void finish()
{
	close(f);
}

int main()
{
	init(0x77);

	bmp085_t x;
	x.bus_read = &bus_read;
	x.bus_write = &bus_write;
	x.delay_msec = &delay;
	bmp085_init(&x);
	bmp085_get_cal_param();
	unsigned long up = bmp085_get_up();
	long pressure = bmp085_get_pressure(up);
	printf("pressure = %ld Pa\n", pressure);

	unsigned short ut = bmp085_get_ut();
	short temp = bmp085_get_temperature(ut);
	float ftemp = 1.0*temp/10.;
	printf("temperature = %f graus C\n", ftemp);

	finish();

	return 0;
}

