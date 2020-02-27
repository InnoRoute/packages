OBJ = tnstatistics.o tnlibstatistics.o
all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

tnstatistics: $(OBJ)

doku:
	indent -br -l150 *.c
	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

