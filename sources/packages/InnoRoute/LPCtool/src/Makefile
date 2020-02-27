obj-m := LPCtool.o
trustnode-objs	:= main.o
#trustnode-objs	:= INR.o INR-NW.o INR-PCI.o
#trustnode-y	+= INR.o
#trustnode-y	+= INR-NW.o
#trustnode-y	+= INR-PCI.o

#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#PWD       := $(shell pwd)
#CC += -g -DDEBUG
all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

doku:
	indent -br -l150 *.c
	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

