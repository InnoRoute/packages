/* bitbang_ft2232.c

   Output some flickering in bitbang mode to the FT2232

   Thanks to max@koeln.ccc.de for fixing and extending
   the example for the second channel.

   This program is distributed under the GPL, version 2
*/
//added fast serial opto mode functionality ulbricht@innoroute.de 2017
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#ifdef __WIN32__
#define sleep(x) Sleep(x)
#endif
#include <libftdi1/ftdi.h>
//#include <ftdi.h>
#define chunksize 8192
int main(int argc, char **argv)
{
    struct ftdi_context *ftdi, *ftdi2;
    unsigned char buf[1];
    int f,i;
    char digibuf[chunksize]={'A'};
    FILE *quelle;

    // Init 2. channel
    if ((ftdi2 = ftdi_new()) == 0)
    {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }
    ftdi_set_interface(ftdi2, INTERFACE_B);
    f = ftdi_usb_open(ftdi2, 0x0403, 0x6010);
    if (f < 0 && f != -5)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi2));
        ftdi_deinit(ftdi2);
        exit(-1);
    }
    printf("ftdi open succeeded(channel 2): %d\n",f);

    ftdi_set_bitmode(ftdi2, 0x00, 0x00);//init
    if(ftdi_set_latency_timer(ftdi2,1))printf("set latencytimer faild\n");
    if(ftdi_usb_purge_buffers(ftdi2))printf("purging buffers fails!\n");
    if(ftdi_write_data_set_chunksize(ftdi2,chunksize))printf("set chuncsize faild\n");
    if(argc != 2) {printf("need filename to write!\n");exit(-1);}
    // Write data
	quelle = fopen(argv[1], "r+b");
	printf("writing %s to opto interface\n",argv[1]);
	uint16_t read=0;
	do{
	if(quelle != NULL){read=fread(&digibuf, 1, chunksize, quelle);
        f = ftdi_write_data(ftdi2, &digibuf, read);
      if (f < 0) fprintf(stderr,"write failed on channel 2: error %d (%s)\n", f, ftdi_get_error_string(ftdi2));  
       }} while (read);
  

    printf("ftdi closing (channel 2)\n");
    ftdi_usb_close(ftdi2);
    ftdi_free(ftdi2);

    return 0;
}
