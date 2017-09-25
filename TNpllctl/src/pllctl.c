#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

void scan_i2c_bus(int device)
  {
  int port, res;

  for (port = 0; port < 127; port++)
    {
    if (ioctl(device, I2C_SLAVE, port) < 0)
      perror("ioctl() I2C_SLAVE failed\n");
    else
      {
      res = i2c_smbus_read_byte(device);
      if (res >= 0)
        printf("i2c chip found at: %x, val = %d\n", port, res);
      }
    }
  }


int test(void)
  {
  int device;
  unsigned long funcs;

  printf("opening device...");
  if ((device = open("/dev/i2c-0", O_RDWR)) < 0)
    {
    perror("open() failed");
    exit (1);
    }
  printf(" OK\n");

  if (ioctl(device,I2C_FUNCS,&funcs) < 0)
    {
    perror("ioctl() I2C_FUNCS failed");
    exit (1);
    }

  if (funcs & I2C_FUNC_I2C)
    printf("I2C\n");
  if (funcs & (I2C_FUNC_SMBUS_BYTE))
    printf("I2C_FUNC_SMBUS_BYTE\n");

  scan_i2c_bus(device);

  return 0;
  }

int ri2c(uint8_t addr, uint16_t reg, uint8_t* buf)
{
   int fd;
  if ((fd = open("/dev/i2c-0", O_RDWR)) < 0) {
    printf("open error! %d\n", fd);
    return -1;
  }
  if (ioctl(fd, I2C_SLAVE, addr) < 0) {
    printf("address error!\n");
    return -1;
  }
i2c_smbus_write_byte_data(fd, reg&0xff,(reg>>8)&0xff);
//  if (write(fd, &reg, 2) != 2) {
//    printf("write error!\n");
//   return -1;
//  }
//  if (read(fd, buf, 1) != 1) {
//    printf("read error!\n");
//    return -1;
//  }
*buf=i2c_smbus_read_byte(fd);
  close(fd);
  return 1;
}

int wi2c(uint8_t addr, uint16_t reg, uint8_t value)
{
  
  uint8_t res, w[2];
  w[0] = (reg)&0xff;
  w[1] = (reg>>8)&0xff;
  w[2] = value;
  int fd;
  if ((fd = open("/dev/i2c-0", O_RDWR)) < 0) {
    printf("open error! %d\n", fd);
    return -1;
  }
  if (ioctl(fd, I2C_SLAVE, addr) < 0) {
    printf("address error!\n");
    return -1;
  }
i2c_smbus_write_word_data(fd, reg&0xff,((uint16_t)value<<8)|((reg>>8)&0xff));

//i2c_smbus_write_byte_data(fd, reg&0xff,(reg>>8)&0xff);
//res=i2c_smbus_read_byte(fd);
/*
  if (write(fd, w, 3) != 3) {
    printf("write error!\n");
    return -1;
  }
  if (read(fd, &res, 1) != 1) {
    printf("read error!\n");
    return -1;
  }
*/

  close(fd);
//  printf("-->0x%02x \n", res);
  return 1;
}

int main(int argc, char const *argv[]) {
  if (argc > 1){
   uint8_t addr = strtol(argv[1], NULL , 16);
   uint16_t reg = strtol(argv[2], NULL , 16);
   printf("0x%02x 0x%04x\n",addr,reg);
   reg=(((reg&0xff)<<8)&0xff00)|(((0xff00&reg)>>8)&0xff);

   uint8_t buf = 0;
   
    if (argc == 4)
    {
      uint8_t value = strtol(argv[3], NULL , 16);
      wi2c(addr, reg, value);
    }// else if (argc == 3) {
      
      ri2c(addr, reg, &buf);
      printf("read-->0x%02x\n", buf);
 //   }
  }else {

 //   printf("test\n");
 //   test();
  }
  return 0;
}
