obj-m := TNflowtable.o
TNflowtable-objs	:= mastertable.o tnlibflowtable.o flowtableactions.o accdpactions.o tnlibaccdp.o
OBJ = mastertable.o tnlibflowtable.o flowtableactions.o tnflowtable.o accdpactions.o tnlibaccdp.o
all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

tnflowtable: $(OBJ)

doku:
	indent -br -l150 *.c
	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

