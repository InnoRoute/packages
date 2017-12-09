obj-m := TN_MMI.o
trustnode-objs	:= main.o

all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

doku:
	astyle *.c
	astyle *.h
#	indent -br -l150 *.c
#	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

