#include "eth_lib.h"


int usage(int argc, char *argv[]){
  printf("Reset the i/o buffers.  Removes all buffered packets.\n");
  printf("Usage: \n");
  printf("%s <device number>\n", argv[0]);
  printf("<device number> = 1 or 2 (for eth1 or eth2)\n");  
  return 1;
}

int main(int argc,char *argv[])
{
  char *tmp;
  char devname[12];

  // check arguments
  if(argc != 2) return usage(argc, argv);

  // set and check device name
  tmp=argv[1];
  if(strcmp(tmp,"1") && strcmp(tmp,"2")) return usage(argc, argv);
  sprintf(devname, "/dev/schar%s",tmp);
  printf("devname %s\n",devname);


  // open device
  eth_open(devname);
  eth_register_mac();






















































































































































































































































































































































  eth_reset();

  // close device
  eth_close();

  return 0;
}
