#include "stdafx.h"
#include <WinTypes.h>
#include <stdio.h>
#include "ftd2xx.h"
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>



// file output
char * timeheader();
// usb open and close routines
int usb_open_232H_device();
int usb_open_2232H_devices();
int usb_configure(FT_HANDLE fthandle);
void usb_close_232H();
void usb_close_2232H();

// test routines
void max_jtag_test();
void max_jtag_test_multi();
void max_jtag_test_after_power_downup();
void virtex6_test();
// jtag routines
int jtag_setup();
void scan(int reg,const char *snd,int cnt,char *rcv,int ird);
void tms_reset();
void print_debug();
void max_idcode(char*snd,char *rcv);
void max_write(char *addr,char *val,char *snd,char *rcv);
void max_read(char *addr,char *val,char *snd,char *rcv);
void clear_buffer();

// jtag multi routines
void Set_Chip_Number_multi(int ibrd);
void tms_reset_multi();
void max_idcode_multi(char *snd,char *rcv);
void max_write_multi(char *addr,char *val,char *snd,char *rcv);
void max_read_multi(char *addr,char *snd,char *rcv);

// gpio routines
void max_gpio_test();
int fta_pbio_write_lowbyte(char input);
int ftb_pbio_read_lowbyte();
int fta_pbio_read_highbyte();
int ftb_pbio_read_highbyte();

// i2c routines
void max_i2c_test();
int i2c_setup_fta();
int setup_ftb();
int max_i2c_write(int dev_add,BYTE reg_address,BYTE data);
int max_i2c_read(int dev_add,BYTE reg_address);
void HighSpeedSetI2CStart(void);
void HighSpeedSetI2CStop(void);
BOOL SendByteAndCheckACK(BYTE dwDataSend);
int i2c_packet();

#define BUF_SIZE 0x10
#define MAX_DEVICES		5
#define Inst_reg 0
#define Data_reg 1

// virtex 6 commands (4 bit commands)
#define VTX6_USR1 0x3c2
#define VTX6_USR2 0x3c3
#define VTX6_USERCODE 0x3c8
#define VTX6_IDCODE 0x3c9
#define VTX6_BYPASS 0x3ff

// maxim commands (4 bit commands)
#define MAX_IDCODE 0x01 
#define MAX_ADDRESS 0x09   
#define MAX_READ 0x0A
#define MAX_WRITE 0x0B
#define MAX_BYPASS 0xFF

	FT_STATUS	ftStatus;
        FT_HANDLE       ftHandle_jtag;
        FT_HANDLE       ftHandle_fta;
        FT_HANDLE       ftHandle_ftb;
        BYTE byOutputBuffer[1024];
        BYTE byInputBuffer[1024];
        BYTE ByteDataRead;
        DWORD dwNumInputBuffer = 0;
        DWORD dwNumBytesToSend = 0;
        DWORD dwNumBytesSent = 0;
	DWORD dwNumBytesToRead = 0;
        DWORD dwNumBytesRead = 0;
	BYTE byOutputBuffer[1024];
	BYTE byInputBuffer[1024];
int idebug;  // 0-no printout 1-essential printout 2-all printout

	const BYTE AA_ECHO_CMD_1 = '\xAA';
	const BYTE AB_ECHO_CMD_2 = '\xAB';
	const BYTE BAD_COMMAND_RESPONSE = '\xFA';

	const BYTE MSB_VEDGE_CLOCK_IN_BIT = '\x22';
	const BYTE MSB_EDGE_CLOCK_OUT_BYTE = '\x11';
	const BYTE MSB_EDGE_CLOCK_IN_BYTE = '\x24';

	const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN = '\x24';
	const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = '\x11';
	const BYTE MSB_DOWN_EDGE_CLOCK_BIT_IN = '\x26';
	const BYTE MSB_UP_EDGE_CLOCK_BYTE_IN = '\x20';
	const BYTE MSB_UP_EDGE_CLOCK_BYTE_OUT = '\x10';
	const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = '\x22';


// All 4 Chips commands                                                         

char ADR4[2];
char RD4[2];
char WR4[2];
char ID4[2];
int ncmd4;
int ndatawr4;
int ndatard4;
int ndataid4;

time_t curt_;
struct tm *loct_;
char thistime[150];
char boardnum[100];

int main(int argc, char *argv[]){
  //printf(" argc %d argv[0] %s \n",argc,argv[1]);
  if(argc!=2){
    printf(" Error: usage ./JTAG 1  where 1 is the board number\n");
    return 1;
  }  
  *boardnum=*argv[1];
  printf(" board number %s \n",boardnum);
  max_jtag_test_multi();
  //max_jtag_test();
  //max_test_after_power_downup();
  //max_i2c_test();
  // max_gpio_test();
  // virtex6_test
  return 1;
}

// jtag routines

void max_jtag_test(){
  char snd[100],rcv[100]={0};
  char addr[100],val[2];
  int i;
  idebug=0;
  usb_open_232H_device();
  usb_configure(ftHandle_jtag);
  jtag_setup();
  tms_reset();
  max_idcode(snd,rcv);
  printf("**** MAX id_code rbk: ");
  for(i=0;i<4;i++)printf("%02x",rcv[3-i]&0xff);
  printf("\n");
  //enable EEPROM
  addr[0]=0xF4;
  val[0]=0x00;
  max_write(addr,val,snd,rcv);
  // disable EEPROM
  //  addr[0]=0xF4;
  //val[0]=0x01;
  //max_write(addr,val,snd,rcv);
  usleep(200000);
  // write and read general purpose user eprom
  addr[0]=0x00;
  val[0]=0xAB;
  max_write(addr,val,snd,rcv);
  usleep(200000);
  clear_buffer();
  addr[0]=0x00;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" **** EEPROM addr 00 rbk 0x%02X ",rcv[0]&0xff);
  printf(" expect 0xAB\n");
 // write pullup io0-7
  printf(" write pullup io 0-7 0xFF\n");
  addr[0]=0xF0;
  val[0]=0xFF;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // write pullup io8
  printf(" write pullup i 8  0xFF\n");
  addr[0]=0xF1;
  val[0]=0x01;
  max_write(addr,val,snd,rcv);
  usleep(200000);
  // write pulldownio 0-7
  printf(" write pulldown io 0-7 0x55\n");
  addr[0]=0xF2;
  val[0]=0x55;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // write pulldown io8
  printf(" write pullup i 8  0x00\n");
  addr[0]=0xF3;
  val[0]=0x00;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // read outputs for io0-7
  printf(" read outputs io 0-7 \n");
  clear_buffer();
  addr[0]=0xF8;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" ****outputs io0-7 0x%02X expect 0x55\n",rcv[0]);
 // read outputs for io8
  printf(" read outputs io 8 \n");
  clear_buffer();
  addr[0]=0xF9;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" ****outputs io8 0x%02X expect 0x00\n",rcv[0]);

 // second round pullups pull downs
 // write pullup io0-7
  printf(" write pullup io 0-7 0xFF\n");
  addr[0]=0xF0;
  val[0]=0xFF;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // write pullup io8
  printf(" write pullup i 8  0xFF\n");
  addr[0]=0xF1;
  val[0]=0x01;
  max_write(addr,val,snd,rcv);
  usleep(200000);
  // write pulldownio 0-7
  printf(" write pulldown io 0-7 0x55\n");
  addr[0]=0xF2;
  val[0]=0x99;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // write pulldown io8
  printf(" write pullup i 8  0x00\n");
  addr[0]=0xF3;
  val[0]=0x01;
  max_write(addr,val,snd,rcv);
  usleep(200000);
 // read outputs for io0-7
  printf(" read outputs io 0-7 \n");
  clear_buffer();
  addr[0]=0xF8;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" **** outputs io0-7 0x%02X expect 0x99\n",rcv[0]);
 // read outputs for io8
  printf(" read outputs io 8 \n");
  clear_buffer();
  addr[0]=0xF9;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" ****outputs io8 0x%02X expect 0x01\n",rcv[0]);
  usb_close_232H();
}

void max_jtag_test_multi(){
  int i,j,itest;
  char snd[100],rcv[100]={0};
  char addr[10],val[10];
  char rcv2[4];
  FILE *Dataout;
  char dout[100];
  char dout2[100];
  sprintf(dout2,"***board %s ***%s",boardnum,timeheader());
  sprintf(dout,"jtag_board_data/jtagrun_brd%s_%s.dat",boardnum,timeheader());
  printf(" %s \n",dout);
  Dataout=fopen(dout,"w");
  idebug=0;
  usb_open_232H_device();
  usb_configure(ftHandle_jtag);
  jtag_setup();
  for(i=1;i<5;i++){
   printf(" %s************ Chip Number %d ************\n",dout2,i);
   fprintf(Dataout,"%s**************** Chip Number %d ************\n",dout2,i);
  // start chip  
  Set_Chip_Number_multi(i);
  if(idebug>=1)printf(" tms reset\n");
    tms_reset_multi(); 

    // idcode 
    max_idcode_multi(snd,rcv);
    if(i==4){
      for(j=0;j<4;j++)rcv2[j]=rcv[j];
    }
    if(i==3){
      for(j=0;j<4;j++)rcv2[j]=((rcv[j]>>1)&0x7f)|((rcv[j+1]<<7)&0x80);
    }
    if(i==2){
      for(j=0;j<4;j++)rcv2[j]=((rcv[j]>>2)&0x3f)|((rcv[j+1]<<6)&0xC0);
    }
    if(i==1){
      for(j=0;j<4;j++)rcv2[j]=((rcv[j]>>3)&0x1f)|((rcv[j+1]<<5)&0xE0);
    }
    itest=1;
    if((rcv2[3]&0xff)==0x01&&(rcv2[2]&0xff)==0x00&&(rcv2[1]&0xff)==0x01&&(rcv2[0]&0xff)==0x43)itest=0;
    printf("**** MAX id_code rbk: ");
    for(j=0;j<4;j++)printf("%02x",rcv2[3-j]&0xff);
    if(itest==1){
      printf(" ******* BAD\n");
    }else{
      printf("\n");
    }
    fprintf(Dataout,"**** MAX id_code rbk: ");
    for(j=0;j<4;j++)fprintf(Dataout,"%02x",rcv2[3-j]&0xff);
    if(itest==1){
      fprintf(Dataout," ******* BAD\n");
    }else{
      fprintf(Dataout,"\n");
    }
  //enable EEPROM write
  if(idebug>=1)printf("enable eprom\n");
  addr[0]=0xF4;
  val[0]=0x00;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
  // write CD to EPROM 0x00
  if(idebug>=1)printf("max_write\n");
  addr[0]=0x00;
  val[0]=0xcd;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
  clear_buffer();
  // read EPROM 0x00
  if(idebug>=1)printf("max_read\n");
  addr[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" ****User EEPROM rbk 0x%02X ",rcv[0]&0xff);
  printf(" Expect 0xCD");
  if((rcv[0]&0xff)!=0xCD){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," ****User EEPROM rbk 0x%02X ",rcv[0]&0xff);
  fprintf(Dataout," Expect 0xCD ");
  if((rcv[0]&0xff)!=0xCD){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  } 
 //enable EEPROM write
  if(idebug>=1)printf("enable eprom\n");
  addr[0]=0xF4;
  val[0]=0x00;  // this should do nothing
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // write CD to EPROM 0x00 2nd Time
  if(idebug>=1)printf("max_write\n");
  addr[0]=0x00;
  val[0]=0xab;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
  clear_buffer();
  // read EPROM 0x00 2nd Time
  if(idebug>=1)printf("max_read\n");
  addr[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" ****User EEPROM rbk 0x%02X ",rcv[0]&0xff);
  printf(" Expect 0xAB");
  if((rcv[0]&0xff)!=0xAB){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," ****User EEPROM rbk 0x%02X ",rcv[0]&0xff);
  fprintf(Dataout," Expect 0xAB ");
  if((rcv[0]&0xff)!=0xAB){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  }
 
 // write pullup io0-7
  if(idebug>=1)printf(" write pullup io 0-7 0xFF\n");
  addr[0]=0xF0;
  val[0]=0xFF;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // write pullup io8
  if(idebug>=1)printf(" write pullup i 8  0xFF\n");
  addr[0]=0xF1;
  val[0]=0x01;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
  // write pulldownio 0-7
  if(idebug>=1)printf(" write pulldown io 0-7 0x55\n");
  addr[0]=0xF2;
  val[0]=0x55;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // write pulldown io8
  if(idebug>=1)printf(" write pullup i 8  0x00\n");
  addr[0]=0xF3;
  val[0]=0x00;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // read outputs for io0-7
  if(idebug>=1)printf(" read outputs io 0-7 \n");
  clear_buffer();
  addr[0]=0xF8;
  val[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" ****outputs io0-7 0x%02X expect 0x55 ",rcv[0&0xff]);
  if((rcv[0]&0xff)!=0x55){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," ****outputs io0-7 0x%02X expect 0x55 ",rcv[0&0xff]);
  if((rcv[0]&0xff)!=0x55){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  }
 
 // read outputs for io8
  if(idebug>=1)printf(" read outputs io 8 \n");
  clear_buffer();
  addr[0]=0xF9;
  val[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" ****outputs io8 0x%02X expect 0x00 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x00){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," ****outputs io8 0x%02X expect 0x00 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x00){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  }
 
 // second round pullups pull downs
 // write pullup io0-7
  if(idebug>=1)printf(" write pullup io 0-7 0xFF\n");
  addr[0]=0xF0;
  val[0]=0xFF;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // write pullup io8
  if(idebug>=1)printf(" write pullup i 8  0xFF\n");
  addr[0]=0xF1;
  val[0]=0x01;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
  // write pulldownio 0-7
  if(idebug>=1)printf(" write pulldown io 0-7 0x55\n");
  addr[0]=0xF2;
  val[0]=0x99;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // write pulldown io8
  if(idebug>=1)printf(" write pullup i 8  0x00\n");
  addr[0]=0xF3;
  val[0]=0x01;
  max_write_multi(addr,val,snd,rcv);
  usleep(200000);
 // read outputs for io0-7
  if(idebug>=1)printf(" read outputs io 0-7 \n");
  clear_buffer();
  addr[0]=0xF8;
  val[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" **** outputs io0-7 0x%02X expect 0x99 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x99){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," **** outputs io0-7 0x%02X expect 0x99 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x99){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  }
 // read outputs for io8
  if(idebug>=1)printf(" read outputs io 8 \n");
  clear_buffer();
  addr[0]=0xF9;
  val[0]=0x00;
  max_read_multi(addr,snd,rcv);
  printf(" ****outputs io8 0x%02X expect 0x01 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x01){
    printf(" ****** Bad\n");
  }else{
    printf("\n");
  }
  fprintf(Dataout," ****outputs io8 0x%02X expect 0x01 ",rcv[0]&0xff);
  if((rcv[0]&0xff)!=0x01){
    fprintf(Dataout," ****** Bad\n");
  }else{
    fprintf(Dataout,"\n");
  }
  }
  usb_close_232H();
  fclose(Dataout);
}




void max_jtag_test_after_power_downup(){
  char snd[100],rcv[100]={0};
  char addr[2],val[2];
  int i;
  idebug=0;
  usb_open_232H_device();
  usb_configure(ftHandle_jtag);
  jtag_setup();
  tms_reset();
  // read max id_code 
  max_idcode(snd,rcv);
  printf(" MAX id_code rbk: ");
  for(i=0;i<4;i++)printf("%02x",rcv[3-i]&0xff);
  printf("\n");
  // read back address 0x00
  clear_buffer();
  addr[0]=0x00;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" User EEPROM rbk 0x%02X ",rcv[0]&0xff);
  printf(" Expect 0xAB\n");
 // read outputs for io0-7
  printf(" read outputs io 0-7 \n");
  clear_buffer();
  addr[0]=0xF8;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" outputs io0-7 0x%02X expect 0x99\n",rcv[0]);
 // read outputs for io8
  printf(" read outputs io 8 \n");
  clear_buffer();
  addr[0]=0xF9;
  val[0]=0x00;
  max_read(addr,val,snd,rcv);
  printf(" outputs io8 0x%02X expect 0x01\n",rcv[0]);
  usb_close_232H();
}

void max_idcode(char*snd,char *rcv){
  // idcode
  //int i;
  // printf(" idcode \n");
  snd[0]=(MAX_IDCODE&0x0f);
  scan(Inst_reg,snd,4,rcv,0);
  snd[0]=0x00;snd[1]=0x00;snd[2]=0x00;snd[3]=0x00;
  snd[4]=0xff;snd[5]=0xff;snd[6]=0xff;snd[7]=0xff;
  scan(Data_reg,snd,32,rcv,1);
  //printf(" MAX id_code: ");
  //for(i=0;i<4;i++)printf("%02x",rcv[3-i]&0xff);
  //printf("\n");
  //  for(i=0;i<4;i++)printf("%02x",rcv[7-i]&0xff);
  //  printf("\n");
}


void max_write(char *addr,char *val,char *snd,char *rcv){
  snd[0]=(MAX_ADDRESS&0x0f);
  scan(Inst_reg,snd,4,rcv,0);
  snd[0]=addr[0];
  scan(Data_reg,snd,8,rcv,0);
  snd[0]=(MAX_WRITE&0x0f);
  scan(Inst_reg,snd,4,rcv,0);
  snd[0]=val[0];
  scan(Data_reg,snd,8,rcv,0);
}

void max_read(char *addr,char *val,char *snd,char *rcv){
  snd[0]=(MAX_ADDRESS&0x0f);
  scan(Inst_reg,snd,4,rcv,0);
  snd[0]=addr[0];
  scan(Data_reg,snd,8,rcv,0);
  snd[0]=(MAX_READ&0x0f);
  scan(Inst_reg,snd,4,rcv,0);
  snd[0]=val[0];
  scan(Data_reg,snd,8,rcv,1);
  //printf(" MAX Read Address %02X Value %02X\n",addr[0]&0xff,rcv[0]&0xff);
}

void Set_Chip_Number_multi(int ibrd){
  int indx,i; 
   char Adr4[2]={0xf9,0xff};
   char Adr3[2]={0x9f,0xff};
   char Adr2[2]={0xff,0xf9};
   char Adr1[2]={0xff,0x9f};
   char Wr4[2]={0xfb,0xff};
   char Wr3[2]={0xbf,0xff};
   char Wr2[2]={0xff,0xfb};
   char Wr1[2]={0xff,0xbf};
   char Rd4[2]={0xfa,0xff};
   char Rd3[2]={0xaf,0xff};
   char Rd2[2]={0xff,0xfa};
   char Rd1[2]={0xff,0xaf};
   char Id4[2]={0xf1,0xff};
   char Id3[2]={0x1f,0xff};
   char Id2[2]={0xff,0xf1};
   char Id1[2]={0xff,0x1f};
  
   int ndatashftwr[4]={8,9,10,11};
   int ndatashftrd[4]={11,10,9,8};
   int ndatashftid[4]={40,40,40,40};
   ncmd4=16;
   indx=ibrd-1;
   ndatawr4=ndatashftwr[indx];
   ndatard4=ndatashftrd[indx];
   ndataid4=ndatashftid[indx];
   if(ibrd==1){
     for(i=0;i<2;i++)ADR4[i]=Adr1[i];
     for(i=0;i<2;i++)RD4[i]=Rd1[i];
     for(i=0;i<2;i++)WR4[i]=Wr1[i];
     for(i=0;i<2;i++)ID4[i]=Id1[i];
   }
   if(ibrd==2){
     for(i=0;i<2;i++)ADR4[i]=Adr2[i];
     for(i=0;i<2;i++)RD4[i]=Rd2[i];
     for(i=0;i<2;i++)WR4[i]=Wr2[i];
     for(i=0;i<2;i++)ID4[i]=Id2[i];
   }
   if(ibrd==3){
     for(i=0;i<2;i++)ADR4[i]=Adr3[i];
     for(i=0;i<2;i++)RD4[i]=Rd3[i];
     for(i=0;i<2;i++)WR4[i]=Wr3[i];
     for(i=0;i<2;i++)ID4[i]=Id3[i];
   }
   if(ibrd==4){
     for(i=0;i<2;i++)ADR4[i]=Adr4[i];
     for(i=0;i<2;i++)RD4[i]=Rd4[i];
     for(i=0;i<2;i++)WR4[i]=Wr4[i];
     for(i=0;i<2;i++)ID4[i]=Id4[i];
   }  
   return;
}

void max_idcode_multi(char *snd,char *rcv){
  // idcode
  //printf(" idcode \n");
  snd[0]=ID4[0];snd[1]=ID4[1];
  scan(Inst_reg,snd,ncmd4,rcv,0);
  snd[0]=0x00;snd[1]=0x00;snd[2]=0x00;snd[3]=0x00;
  snd[4]=0x00;snd[5]=0x00;
  scan(Data_reg,snd,ndataid4,rcv,1);
  //printf(" MAX id_code: ");
  //for(i=0;i<6;i++)printf("%02x",rcv[i]&0xff);
  //printf("\n");
  // for(i=0;i<4;i++)printf("%02x",rcv[7-i]&0xff);
  //printf("\n");
}

void tms_reset_multi(){
  int i;
  if(idebug==1)printf(" TMS reset multi\n");
     dwNumBytesToSend = 0;
     for(i=0;i<4;i++){
     byOutputBuffer[dwNumBytesToSend++] = 0x4B;
     byOutputBuffer[dwNumBytesToSend++] = 0x04;
     byOutputBuffer[dwNumBytesToSend++] = 0xFF;
     }
     print_debug();
     ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
}


void max_write_multi(char *addr,char *val,char *snd,char *rcv){
  snd[0]=ADR4[0];snd[1]=ADR4[1];
  if(idebug==2)printf(" write:Inst %d %02x%02x\n",ncmd4,snd[1]&0xff,snd[0]&0xff); 
  scan(Inst_reg,snd,ncmd4,rcv,0);
  snd[0]=addr[0];
  scan(Data_reg,snd,ndatawr4,rcv,0);
  snd[0]=WR4[0];snd[1]=WR4[1];
  if(idebug==2)printf(" write:Data %d %02x%02x\n",ndatawr4,snd[1]&0xff,snd[0]&0xff);
  scan(Inst_reg,snd,ncmd4,rcv,0);
  snd[0]=val[0];
  scan(Data_reg,snd,ndatawr4,rcv,0);
  if(idebug==2)printf(" end write\n");
}

void max_read_multi(char *addr,char *snd,char *rcv){
  snd[0]=ADR4[0];snd[1]=ADR4[1];snd[2]=ADR4[2];
  if(idebug==2)printf(" write:Inst %d %02x%02x\n",ncmd4,snd[1]&0xff,snd[0]&0xff); 
  scan(Inst_reg,snd,ncmd4,rcv,0);
  snd[0]=addr[0];
  scan(Data_reg,snd,ndatawr4,rcv,0);
  snd[0]=RD4[0];snd[1]=RD4[1];
  if(idebug==2)printf(" write:Data %d %02x%02x\n",ndatawr4,snd[1]&0xff,snd[0]&0xff);
  scan(Inst_reg,snd,ncmd4,rcv,0);
  snd[0]=0xff;snd[2]=0xff;snd[3]=0xff;snd[4]=0xff;
  scan(Data_reg,snd,ndatard4,rcv,1);
printf(" MAX Read Address %02X Value %02X\n",addr[0]&0xff,rcv[0]&0xff);
}



void virtex6_test(){
  char snd[100],rcv[100]={0};
  int i;
  idebug=0;
  usb_open_232H_device();
  jtag_setup();
  tms_reset();
  // read idcode
  snd[0]=(VTX6_IDCODE&0xff);
  snd[1]=((VTX6_IDCODE&0x300)>>8);
  scan(Inst_reg,snd,10,rcv,0);
  snd[0]=0xff;snd[1]=0xff;snd[2]=0xff;snd[3]=0xff;
  scan(Data_reg,snd,32,rcv,1);
  printf(" virtex 6 id_code: ");
  for(i=0;i<4;i++)printf("%02x",rcv[3-i]&0xff);
  printf("\n");

  // read usrcode
  snd[0]=(VTX6_USERCODE&0xff);
  snd[1]=((VTX6_USERCODE&0x300)>>8);
  scan(Inst_reg,snd,10,rcv,0);
  snd[0]=0xff;snd[1]=0xff;snd[2]=0xff;snd[3]=0xff;
  scan(Data_reg,snd,32,rcv,1);
  printf(" virtex 6 user_code: ");
  for(i=0;i<4;i++)printf("%02x",rcv[3-i]&0xff);
  printf("\n");

  usb_close_232H();
  return;
}

int usb_open_232H_device()
{
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
        long locIdBuf[MAX_DEVICES];	
        long shuf[3];
        long temp;
        int jtemp;
        int jshuf[3]={0,1,2};
        int	iNumDevs = 0;
        int i,j;
        int ierr;

	// Does an FTDI device exist?

	printf("Checking for FTDI devices...\n");
        //ierr=system("rmmod ftdi_sio");
	// ierr=system("lsmod | grep ftdi_sio");
	// printf(" after lsmod \n");
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	pcBufLD[MAX_DEVICES] = NULL;
	ftStatus = FT_ListDevices(locIdBuf, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_LOCATION);
	//ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
        printf(" number of devices found %d\n",iNumDevs);	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", (int)ftStatus);
		return 1;
	}
        if(iNumDevs==1){
          shuf[0]=locIdBuf[0];
          printf(" Device %d LocID:%x \r\n",i,(unsigned int)locIdBuf[i]);
        }else{
          for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
	    printf(" Device %d LocID:%x \r\n",i,(unsigned int)locIdBuf[i]);
            shuf[i]=locIdBuf[i];
            jshuf[i]=i;
          }
          for(i=0;i<2;i++){
            for(j=i+1;j<3;j++){
              if(shuf[i]>shuf[j]){
                  temp=shuf[j];
                  jtemp=jshuf[j];
                  shuf[j]=shuf[i];
                  jshuf[j]=jshuf[i];
                  shuf[i]=temp;
                  jshuf[i]=jtemp;
                }
	      }
	    } 
	  for(i=0;i<3;i++)printf(" %d %x %d \n",i,(unsigned int)shuf[i],jshuf[i]);
	}
	//for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
	// printf("Device %d Serial Number - %s  %02x\n", i, cBufLD[i],cBufLD[i][0]&0xff);
	// if(cBufLD[i][0]==0x32)iopen=i;
	// }
        //printf(" iopen %d  \n",iopen);
        //for(i=0;i<12;i++)printf("%c",cBufLD[iopen][i]);
	// Open the port - For this application note, we'll assume the first device is a FT2232H or FT4232H


       
	
	/*
        ftStatus=FT_OpenEx((PVOID)shuf[1],FT_OPEN_BY_LOCATION,&ftHandle_fta);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_fta with error %d\n", ftStatus);
	       	return 1;					// Exit with error
	}	
	printf(" ftstatus fthandle_fta: %d\n",ftStatus);

        ftStatus=FT_OpenEx((PVOID)shuf[2],FT_OPEN_BY_LOCATION,&ftHandle_ftb);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_ftb with error %d\n", ftStatus);
	       	return 1;					// Exit with error
	}	
	printf(" ftstatus fthandle_ftb:%d\n",ftStatus);
	*/
	
        ierr=system(" lsmod ftdi_sio");

	ftStatus=FT_OpenEx((PVOID)shuf[0],FT_OPEN_BY_LOCATION,&ftHandle_jtag);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_jtag with error %d\n", ftStatus);
	     	return 1;					// Exit with error
	} 	
	printf(" ftstatus fthandle_jtag: %d\n",ftStatus);
 

       //ftStatus = FT_OpenEx(cBufLD[iopen], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	//ftStatus = FT_OpenEx("A", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	//ftStatus = FT_OpenEx("210249A065F8", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
        //ftHandle=ftHandlet[iopen];
        return 1;
}
int usb_open_2232H_devices()
{
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
        long locIdBuf[MAX_DEVICES];	
        long shuf[3];
        long temp;
        int jtemp;
        int jshuf[3]={0,1,2};
        int	iNumDevs = 0;
        int i,j;

	// Does an FTDI device exist?

	printf("Checking for FTDI devices...\n");
        //ierr=system("rmmod ftdi_sio");
	// ierr=system("lsmod | grep ftdi_sio");
        printf(" after lsmod \n");
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	pcBufLD[MAX_DEVICES] = NULL;
	ftStatus = FT_ListDevices(locIdBuf, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_LOCATION);
	//ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
        printf(" number of devices found %d\n",iNumDevs);	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", (int)ftStatus);
		return 1;
	}
        if(iNumDevs==1){
          shuf[0]=locIdBuf[0];
        }else{
          for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
	    printf(" Device %d LocID:%x \r\n",i,(unsigned int)locIdBuf[i]);
            shuf[i]=locIdBuf[i];
            jshuf[i]=i;
          }
          for(i=0;i<2;i++){
            for(j=i+1;j<3;j++){
              if(shuf[i]>shuf[j]){
                  temp=shuf[j];
                  jtemp=jshuf[j];
                  shuf[j]=shuf[i];
                  jshuf[j]=jshuf[i];
                  shuf[i]=temp;
                  jshuf[i]=jtemp;
                }
	      }
	    } 
	  for(i=0;i<3;i++)printf(" %d %x %d \n",i,(unsigned int)shuf[i],jshuf[i]);
	}
	//for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
	// printf("Device %d Serial Number - %s  %02x\n", i, cBufLD[i],cBufLD[i][0]&0xff);
	// if(cBufLD[i][0]==0x32)iopen=i;
	// }
        //printf(" iopen %d  \n",iopen);
        //for(i=0;i<12;i++)printf("%c",cBufLD[iopen][i]);
	// Open the port - For this application note, we'll assume the first device is a FT2232H or FT4232H


        ftStatus=FT_OpenEx((PVOID)shuf[1],FT_OPEN_BY_LOCATION,&ftHandle_fta);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_fta with error %d\n", ftStatus);
	       	return 1;					// Exit with error
	}	
	printf(" ftstatus fthandle_fta: %d\n",ftStatus);

        ftStatus=FT_OpenEx((PVOID)shuf[2],FT_OPEN_BY_LOCATION,&ftHandle_ftb);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_ftb with error %d\n", ftStatus);
	       	return 1;					// Exit with error
	}	
	printf(" ftstatus fthandle_ftb:%d\n",ftStatus);
	
        //ierr=system("insmod /lib/modules/2.6.32-642.15.1.el6.i686/kernel/drivers/usb/serial/ftdi_sio.ko");
        
	ftStatus=FT_OpenEx((PVOID)shuf[0],FT_OPEN_BY_LOCATION,&ftHandle_jtag);	        if (ftStatus != FT_OK)
	{
		printf("Open Failed fthandle_jtag with error %d\n", ftStatus);
	     	return 1;					// Exit with error
	} 	
	printf(" ftstatus fthandle_jtag: %d\n",ftStatus);
 

       //ftStatus = FT_OpenEx(cBufLD[iopen], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	//ftStatus = FT_OpenEx("A", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
	//ftStatus = FT_OpenEx("210249A065F8", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);
        //ftHandle=ftHandlet[iopen];
        return 1;
}

int usb_configure(FT_HANDLE ftHandle){

	// Configure port parameters
	
	printf("\nConfiguring port for MPSSE use...\n");

	ftStatus |= FT_ResetDevice(ftHandle); 
								//Reset USB device
	
	//Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
	ftStatus |= FT_GetQueueStatus(ftHandle, &dwNumBytesToRead); 
									// Get the number of bytes in the FT2232H receive buffer
	if ((ftStatus == FT_OK) && (dwNumBytesToRead > 0))
		FT_Read(ftHandle, &byInputBuffer, dwNumBytesToRead, &dwNumBytesRead); 

								//Read out the data from FT2232H receive buffer
	ftStatus |= FT_SetUSBParameters(ftHandle, 65536, 65535);

									//Set USB request transfer sizes to 64K
	ftStatus |= FT_SetChars(ftHandle, false, 0, false, 0);

									//Disable event and error characters
	ftStatus |= FT_SetTimeouts(ftHandle, 0, 5000);

									//Sets the read and write timeouts in milliseconds 
	ftStatus |= FT_SetLatencyTimer(ftHandle, 16);

									//Set the latency timer (default is 16mS)
	ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00);

									//Reset controller
	ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02); 

									//Enable MPSSE mode
	if (ftStatus != FT_OK)
	{
		printf("Error in initializing the MPSSE %d\n", ftStatus);
		FT_Close(ftHandle);
		return 1;					// Exit with error
	}	

	usleep(5000); // Wait for all the USB stuff to complete and work	
	return 1;
}

int jtag_setup(){
        DWORD dwCount = 0;
	DWORD dwClockDivisor = 0x05DB;	// Value of clock divisor, SCL Frequency = 60/((1+0x05DB)*2) (MHz) = 20khz

	// -----------------------------------------------------------
	// Synchronize the MPSSE by sending a bogus opcode (0xAA), 
	//		The MPSSE will respond with "Bad Command" (0xFA) followed by
	//		the bogus opcode itself.
	// -----------------------------------------------------------

	
	byOutputBuffer[dwNumBytesToSend++] = 0xAA;//'\xAA';
									//Add bogus command xAA to the queue
	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the BAD commands
	dwNumBytesToSend = 0;			// Reset output buffer pointer
	do
	{
		ftStatus = FT_GetQueueStatus(ftHandle_jtag, &dwNumBytesToRead); 
									// Get the number of bytes in the device input buffer
	} while ((dwNumBytesToRead == 0) && (ftStatus == FT_OK)); 
									//or Timeout

	bool bCommandEchod = false;
	
	ftStatus = FT_Read(ftHandle_jtag, &byInputBuffer, dwNumBytesToRead, &dwNumBytesRead); 
									//Read out the data from input buffer
	for (dwCount = 0; dwCount < dwNumBytesRead - 1; dwCount++) 
									//Check if Bad command and echo command received
	{
		if ((byInputBuffer[dwCount] == 0xFA) && (byInputBuffer[dwCount+1] == 0xAA))
		{
			bCommandEchod = true;
			break;
		}
	}
	if (bCommandEchod == false)
	{
		printf("Error in synchronizing the MPSSE\n");
		FT_Close(ftHandle_jtag);
		return 1;					// Exit with error
	}	

	/* 
	// -----------------------------------------------------------
	// Configure the MPSSE settings for JTAG
	//		Multple commands can be sent to the MPSSE with one FT_Write
	// -----------------------------------------------------------
	*/
	dwNumBytesToSend = 0;			// Start with a fresh index
	

	// Set up the Hi-Speed specific commands for the FTx232H

	byOutputBuffer[dwNumBytesToSend++] = 0x8A;
									// Use 60MHz master clock (disable divide by 5)
	byOutputBuffer[dwNumBytesToSend++] = 0x97;
									// Turn off adaptive clocking (may be needed for ARM)
	byOutputBuffer[dwNumBytesToSend++] = 0x8D;
									// Disable three-phase clocking
	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the HS-specific commands
	
	dwNumBytesToSend = 0;			// Reset output buffer pointer
	
	// Set initial states of the MPSSE interface - low byte, both pin directions and output values
	//		Pin name	Signal	Direction	Config	Initial State	Config
	//		ADBUS0		TCK		output		1		low				0
	//		ADBUS1		TDI		output		1		low				0
	//		ADBUS2		TDO		input		0						0
	//		ADBUS3		TMS		output		1		high			1
	//		ADBUS4		GPIOL0	input		0						0
	//		ADBUS5		GPIOL1	input		0						0
	//		ADBUS6		GPIOL2	input		0						0
	//		ADBUS7		GPIOL3	input		0						0
			
	byOutputBuffer[dwNumBytesToSend++] = 0x80;
									// Set data bits low-byte of MPSSE port
	byOutputBuffer[dwNumBytesToSend++] = 0xe8;
									// Initial state config above
	byOutputBuffer[dwNumBytesToSend++] = 0xeB;
									// Direction config above
	
	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the low GPIO config commands

	dwNumBytesToSend = 0;			// Reset output buffer pointer


	// Set initial states of the MPSSE interface - high byte, both pin directions and output values
	//		Pin name	Signal	Direction	Config	Initial State	Config
	//		ACBUS0		GPIOH0	input		0						0
	//		ACBUS1		GPIOH1	input		0						0
	//		ACBUS2		GPIOH2	input		0						0
	//		ACBUS3		GPIOH3	input		0						0
	//		ACBUS4		GPIOH4	input		0						0
	//		ACBUS5		GPIOH5	input		0						0
	//		ACBUS6		GPIOH6	input		0						0
	//		ACBUS7		GPIOH7	input		0						0

	byOutputBuffer[dwNumBytesToSend++] = 0x82;
									// Set data bits low-byte of MPSSE port
	byOutputBuffer[dwNumBytesToSend++] = 0x00;
									// Initial state config above
	byOutputBuffer[dwNumBytesToSend++] = 0x00;
									// Direction config above

	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the high GPIO config command

	dwNumBytesToSend = 0;			// Reset output buffer pointer
	
	// Set TCK frequency 
	// TCK = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)

	byOutputBuffer[dwNumBytesToSend++] = 0x86; //'\x86'; 
									//Command to set clock divisor
	byOutputBuffer[dwNumBytesToSend++] = dwClockDivisor & 0xFF; 
									//Set 0xValueL of clock divisor
	byOutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & 0xFF; 
									//Set 0xValueH of clock divisor
	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the clock divisor commands

	dwNumBytesToSend = 0;			// Reset output buffer pointer


	// Disable internal loop-back

	byOutputBuffer[dwNumBytesToSend++] = 0x85;
									// Disable loopback

	ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent); 
									// Send off the loopback command
	return 1;
}

// JTAG ROUTINEs

void scan(int reg,const char *snd,int cnt,char *rcv,int ird){
  int i,j;
  int byte,bit;
  BYTE *data;
  char temp; 
  if(cnt==0)return;
  data=(BYTE *)snd;
  /* instr*/
  if(reg==0){
     dwNumBytesToSend = 0;
     byOutputBuffer[dwNumBytesToSend++] = 0x4B;
     byOutputBuffer[dwNumBytesToSend++] = 0x05;
     byOutputBuffer[dwNumBytesToSend++] = 0x0C;
     print_debug();
     ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
      dwNumBytesToSend = 0;		       
  }
  /* data */
  if(reg==1){
     dwNumBytesToSend = 0;
     byOutputBuffer[dwNumBytesToSend++] = 0x4B;
     byOutputBuffer[dwNumBytesToSend++] = 0x04;
     byOutputBuffer[dwNumBytesToSend++] = 0x04;
     print_debug();
     ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  } 
  dwNumBytesToSend = 0;
  byte=(cnt-1)/8;
  //if(ird==1)byte=byte+1;
  bit=cnt-byte*8-1;
  if(ird==1){
     temp = 0x3B;
   } else {
    temp = 0x1B;
   }
  for(i=0;i<byte;i++){
    byOutputBuffer[dwNumBytesToSend++] = temp;
    byOutputBuffer[dwNumBytesToSend++] = 0x07;
    byOutputBuffer[dwNumBytesToSend++] = data[i];
  }
  if(bit>=1){
    byOutputBuffer[dwNumBytesToSend++] = temp;
    byOutputBuffer[dwNumBytesToSend++] = bit-1;
    byOutputBuffer[dwNumBytesToSend++] = data[byte];
  }
  print_debug();

  ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x6B;
  byOutputBuffer[dwNumBytesToSend++] = 0x00;
  //byOutputBuffer[dwNumBytesToSend++] = (data[byte]&0x80)|0x01;
    if(((data[byte]>>bit)&0x01)==0x01){
      byOutputBuffer[dwNumBytesToSend++] = 0x81;
    }else{
      byOutputBuffer[dwNumBytesToSend++] = 0x01;
    }
  //byOutputBuffer[dwNumBytesToSend++] = (data[byte]>>bit) & 0x01;
  //  byOutputBuffer[dwNumBytesToSend++] = 0x01;
  //  byOutputBuffer[dwNumBytesToSend++] = data[byte];
  print_debug();
  ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x4B;
  byOutputBuffer[dwNumBytesToSend++] = 0x01;
  byOutputBuffer[dwNumBytesToSend++] = 0x01;
  print_debug();
  ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  if(ird==1){
        dwNumBytesToRead = 0;
        dwNumBytesRead=0;
	do
	{
		ftStatus = FT_GetQueueStatus(ftHandle_jtag, &dwNumBytesToRead); 
	} while ((dwNumBytesToRead == 0) && (ftStatus == FT_OK));
	ftStatus = FT_Read(ftHandle_jtag, &byInputBuffer, dwNumBytesToRead, &dwNumBytesRead);
        if(idebug>=1){
          printf(" dwNumBytesToRead %d \n",dwNumBytesToRead);
          printf(" dwNumBytesRead %d \n",dwNumBytesRead);
        }
        i=dwNumBytesToRead;
	if(idebug>=1)for(j=0;j<i;j++)printf(" %d %02x\n",j,byInputBuffer[j]);
	for(j=1;j<(cnt/8+1);j++)rcv[j-1]=(char) byInputBuffer[j];
        rcv[cnt/8-1]=byInputBuffer[i-1];
     }

} 

void clear_buffer(){
  ftStatus = FT_GetQueueStatus(ftHandle_jtag, &dwNumBytesToRead); 
  if(idebug>=1){
    printf("clear_buffer: before -- dwNumBytesToRead %d \n",dwNumBytesToRead);
    printf("clear_buffer: before -- dwNumBytesRead %d \n",dwNumBytesRead);
  }
  if((dwNumBytesToRead != 0) && (ftStatus == FT_OK)){
    ftStatus = FT_Read(ftHandle_jtag, &byInputBuffer, dwNumBytesToRead, &dwNumBytesRead);
  }
  if(idebug>=1){
    printf("clear_buffer: after  -- dwNumBytesToRead %d \n",dwNumBytesToRead);
    printf("clear_buffer: after  -- dwNumBytesRead %d \n",dwNumBytesRead);
  }
}
	
void print_debug(){
  int i,j;
  if(idebug<2)return;
  //printf(" debug output \n");
  j=dwNumBytesToSend/3;
  for(i=0;i<j;i++){
    printf(" %02x %02x %02x\n",byOutputBuffer[i*3],byOutputBuffer[i*3+1],byOutputBuffer[i*3+2]);
  }

}

void usb_close_232H(){
    FT_Close(ftHandle_jtag);
}

void usb_close_2232H(){
        FT_Close(ftHandle_fta);
        FT_Close(ftHandle_ftb);
}

void tms_reset(){
  if(idebug==1)printf(" TMS reset\n");
     dwNumBytesToSend = 0;
     byOutputBuffer[dwNumBytesToSend++] = 0x4B;
     byOutputBuffer[dwNumBytesToSend++] = 0x04;
     byOutputBuffer[dwNumBytesToSend++] = 0xFF;
     print_debug();
     ftStatus = FT_Write(ftHandle_jtag, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
}

// END JTAG ROUTINES

// START I2C ROUTINES

void max_i2c_test(){
 idebug=1;
 usb_open_2232H_devices();
 usb_configure(ftHandle_fta);
 usb_configure(ftHandle_ftb);
 i2c_setup_fta();
 setup_ftb();
 max_i2c_write(1,0xf4,0x00);
 usleep(2000000);
 printf(" start max_i2c_write\n");
 max_i2c_write(1,0x01,0x23);
 printf(" end max_i2c_write\n");
 return;
 usleep(200000);
 printf(" start max_i2c_write\n");
 max_i2c_read(1,0x01);
 printf(" end max_i2c_write\n");
 usb_close_2232H();
}
void max_gpbio_test(){
  idebug=1;
 usb_open_2232H_devices();
 usb_configure(ftHandle_fta);
 usb_configure(ftHandle_ftb);
 i2c_setup_fta();
 setup_ftb();
 fta_pbio_write_lowbyte(0xaa);
 ftb_pbio_read_lowbyte();
 usb_close_2232H();
}


int i2c_setup_fta(){
  // DWORD dwCount = 0;
 DWORD dwClockDivisor = 0x05DB;
 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x8A; //'\x8A';
 byOutputBuffer[dwNumBytesToSend++] = 0x97; //'\x97';
 byOutputBuffer[dwNumBytesToSend++] = 0x8D; //'\x8D';
 ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);

 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80';
 byOutputBuffer[dwNumBytesToSend++] = 0x03; //'\x03';
 byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';
 ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x82; //'\x82';
 byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';
 byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';
 ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);

 byOutputBuffer[dwNumBytesToSend++] = 0x86; //'\x86';
 byOutputBuffer[dwNumBytesToSend++] = dwClockDivisor & 0xFF; //'\xFF';
 byOutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & 0xFF; //'\xFF';
 ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 usleep(200000);

 byOutputBuffer[dwNumBytesToSend++] = 0x85; //'\x85';
 ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 usleep(200000);
 return 1;
}

int setup_ftb(){
  //DWORD dwCount = 0;
 DWORD dwClockDivisor = 0x05DB;
 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x8A; //'\x8A';
 byOutputBuffer[dwNumBytesToSend++] = 0x97; //'\x97';
 byOutputBuffer[dwNumBytesToSend++] = 0x8D; //'\x8D';
 ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);

 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80';
 byOutputBuffer[dwNumBytesToSend++] = 0x03; //'\x03';
 byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';
 byOutputBuffer[dwNumBytesToSend++] = 0x0D; //'\x0D';
 ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 byOutputBuffer[dwNumBytesToSend++] = 0x82; //'\x82';
 byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';
 byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';
 ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);

 byOutputBuffer[dwNumBytesToSend++] = 0x86; //'\x86';
 byOutputBuffer[dwNumBytesToSend++] = dwClockDivisor & 0xFF; //'\xFF';
 byOutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & 0xFF; //'\xFF';
 ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 usleep(20000);

 byOutputBuffer[dwNumBytesToSend++] = 0x85; //'\x85';
 ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
 dwNumBytesToSend = 0;
 usleep(20000);
 return 1;
}

void HighSpeedSetI2CStart(void)
{
	DWORD dwCount;
	for(dwCount=0; dwCount < 4; dwCount++)	// Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
	{
	  byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80';	//Command to set directions of lower 8 pins and force value on bits set as output
	  byOutputBuffer[dwNumBytesToSend++] = 0x03; //'\x03';  //Set SDA, SCL high, WP disabled by SK, DO at bit ¡®1¡¯, GPIOL0 at bit ¡®0¡¯
	  byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
	}
	for(dwCount=0; dwCount < 4; dwCount++)	// Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
	{
	  byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	  byOutputBuffer[dwNumBytesToSend++] = 0x01; //'\x01'; 	//Set SDA low, SCL high, WP disabled by SK at bit ¡®1¡¯, DO, GPIOL0 at bit ¡®0¡¯
	  byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
	}
	byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00'; 	//Set SDA, SCL low high, WP disabled by SK, DO, GPIOL0 at bit ¡®0¡¯
	byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
}

void HighSpeedSetI2CStop(void)
{
	DWORD dwCount;
	for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
	{
	  byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	  byOutputBuffer[dwNumBytesToSend++] = 0x01; //'\x01'; 	//Set SDA low, SCL high, WP disabled by SK at bit ¡®1¡¯, DO, GPIOL0 at bit ¡®0¡¯
	  byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
	}
	for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
	{
	  byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	  byOutputBuffer[dwNumBytesToSend++] = 0x03; //'\x03'; 	//Set SDA, SCL high, WP disabled by SK, DO at bit ¡®1¡¯, GPIOL0 at bit ¡®0¡¯
	  byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
	}
	//Tristate the SCL, SDA pins
	byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00'; 	//Set WP disabled by GPIOL0 at bit 0¡¯
	byOutputBuffer[dwNumBytesToSend++] = 0x10; //'\x10';	//Set GPIOL0 pins as output with bit ¡¯1¡¯, SK, DO and other pins as input with bit ¡®0¡¯
}

BOOL SendByteAndCheckACK(BYTE dwDataSend)
{
        printf("dwDataSend %02x\n",dwDataSend);
	FT_STATUS ftStatus = FT_OK;
	byOutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_OUT; 	//clock data byte out on ¨Cve Clock Edge MSB first
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00';	//Data length of 0x0000 means 1 byte data to clock out
	byOutputBuffer[dwNumBytesToSend++] = dwDataSend;	//Set control byte, bit 4-7 of ¡®1010¡¯ as 24LC02 control code, bit 1-3 as block select bits  //which is don¡¯t care here, bit 0 of ¡®0¡¯ represent Write operation
	//Get Acknowledge bit from EEPROM
	byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x00'; 	//Set SCL low, WP disabled by SK, GPIOL0 at bit ¡®0¡¯
	byOutputBuffer[dwNumBytesToSend++] = 0x11; //'\x11';	//Set SK, GPIOL0 pins as output with bit ¡¯1¡¯, DO and other pins as input with bit ¡®0¡¯
	byOutputBuffer[dwNumBytesToSend++] = MSB_RISING_EDGE_CLOCK_BIT_IN; 	//Command to scan in acknowledge bit , -ve clock Edge MSB first
	byOutputBuffer[dwNumBytesToSend++] = 0x00; //'\x0';	//Length of 0x0 means to scan in 1 bit

	byOutputBuffer[dwNumBytesToSend++] = 0x87; //'\x87';	//Send answer back immediate command
	ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);		//Send off the commands
	dwNumBytesToSend = 0;			//Clear output buffer
	//Check if ACK bit received, may need to read more times to get ACK bit or fail if timeout
        usleep(200000);
	ftStatus = FT_Read(ftHandle_fta, byInputBuffer, 1, &dwNumBytesRead);  	//Read one byte from device receive buffer
        printf(" bytes read for device %d byInputBuffer %02x ftstatus %d\n",dwNumBytesRead,byInputBuffer[0],ftStatus);
	if ((ftStatus != FT_OK) || (dwNumBytesRead == 0))
	{
		printf("fail to get ACK when send control byte 1 [Program Section] \n");
			return FALSE; //Error, can't get the ACK bit from EEPROM
	}
	else 
	{
		if (((byInputBuffer[0] & 0x01)  != 0x00)	)	//Check ACK bit 0 on data byte read out
		{	
			printf("fail to get ACK when send control byte 2 [Program Section] \n");
			return FALSE; //Error, can't get the ACK bit from EEPROM
		}
	}
	
	byOutputBuffer[dwNumBytesToSend++] = 0x80; //'\x80'; 	//Command to set directions of lower 8 pins and force value on bits set as output
	byOutputBuffer[dwNumBytesToSend++] = 0x02; //'\x02'; 	//Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
	byOutputBuffer[dwNumBytesToSend++] = 0x13; //'\x13';	//Set SK,DO,GPIOL0 pins as output with bit ¡¯1¡¯, other pins as input with bit ¡®0¡¯
	return TRUE;
}

int fta_pbio_write_lowbyte(char input){
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x80;
  byOutputBuffer[dwNumBytesToSend++] = 0x0B|input;
  byOutputBuffer[dwNumBytesToSend++] = 0xFB;
  ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  return 0;
}

int ftb_pbio_read_lowbyte(){
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x81; //'\x81';
  ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
   usleep(200000);
   ftStatus = FT_GetQueueStatus(ftHandle_ftb, &dwNumBytesToRead);
   if((ftStatus != FT_OK) & (dwNumBytesToRead != 1))
   {
     printf("Error-GPIO cannot be read \n");
     FT_SetBitMode(ftHandle_ftb, 0x0, 0x00);
     // Reset the port to disable MPSSE
     FT_Close(ftHandle_ftb);
     // Close the USB port
     return 1;
     // Exit with error
    }
    printf( "The GPIO lowbyte = 0x%X \n",byInputBuffer[0]);
    return 0;
}

int fta_pbio_read_highbyte(){
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x83; //'\x83';
  ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
   usleep(200000);
   ftStatus = FT_GetQueueStatus(ftHandle_fta, &dwNumBytesToRead);
   if((ftStatus != FT_OK) & (dwNumBytesToRead != 1))
   {
     printf("Error-GPIO cannot be read \n");
     FT_SetBitMode(ftHandle_fta, 0x0, 0x00);
     // Reset the port to disable MPSSE
     FT_Close(ftHandle_fta);
     // Close the USB port
     return 1;
     // Exit with error
    }
    printf( "The A port GPIO highbyte = 0x%X \n",byInputBuffer[0]);
    return 0;
}

int ftb_pbio_read_highbyte(){
  dwNumBytesToSend = 0;
  byOutputBuffer[dwNumBytesToSend++] = 0x83; //'\x83';
  ftStatus = FT_Write(ftHandle_ftb, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
   usleep(200000);
   ftStatus = FT_GetQueueStatus(ftHandle_ftb, &dwNumBytesToRead);
   if((ftStatus != FT_OK) & (dwNumBytesToRead != 1))
   {
     printf("Error-GPIO cannot be read \n");
     FT_SetBitMode(ftHandle_ftb, 0x0, 0x00);
     // Reset the port to disable MPSSE
     FT_Close(ftHandle_ftb);
     // Close the USB port
     return 1;
     // Exit with error
    }
    printf( "The A port GPIO highbyte = 0x%X \n",byInputBuffer[0]);
    return 0;
}

int max_i2c_write(int dev_add,BYTE reg_address,BYTE data){
  BYTE device[5]={0x00,0xA2,0xA4,0xA6,0xA8};
  BOOL bSucceed;
  printf(" i2c_write: %02X %02X %02X\n",device[dev_add],reg_address,data);
  dwNumBytesToSend = 0;
  HighSpeedSetI2CStart();
  printf(" send device\n");
  bSucceed = SendByteAndCheckACK(device[dev_add]);
  printf(" send address\n");
  bSucceed = SendByteAndCheckACK(reg_address);
  printf(" send data\n");
  bSucceed = SendByteAndCheckACK(data);
  HighSpeedSetI2CStop();
  ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
  return 0;
}

int max_i2c_read(int dev_add,BYTE reg_address){
  BYTE dev[4]={0xA2,0xA4,0xA6,0xA8};
  BYTE dev2[4]={0xA3,0xA5,0xA7,0xA9};
  BOOL bSucceed;
  // purge usb recieve buffer 
  ftStatus = FT_GetQueueStatus(ftHandle_fta, &dwNumInputBuffer);
if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))FT_Read(ftHandle_fta, &byInputBuffer, dwNumInputBuffer, &dwNumBytesRead);

  dwNumBytesToSend = 0;
  HighSpeedSetI2CStart();
  bSucceed = SendByteAndCheckACK(dev[dev_add]);
  bSucceed = SendByteAndCheckACK(reg_address);
  HighSpeedSetI2CStart();
  bSucceed = SendByteAndCheckACK(dev2[dev_add]);
  byOutputBuffer[dwNumBytesToSend++] = 0x80;
  byOutputBuffer[dwNumBytesToSend++] = 0x00; 	
  byOutputBuffer[dwNumBytesToSend++] = 0x11; 
  byOutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_IN;   
  byOutputBuffer[dwNumBytesToSend++] = 0x00;
  byOutputBuffer[dwNumBytesToSend++] = 0x00;    
  byOutputBuffer[dwNumBytesToSend++] = 0x87;
  byOutputBuffer[dwNumBytesToSend++] = MSB_RISING_EDGE_CLOCK_BIT_IN; 	
  byOutputBuffer[dwNumBytesToSend++] = 0x00;       
  byOutputBuffer[dwNumBytesToSend++] = 0x87;
  ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;			
  ftStatus = FT_Read(ftHandle_fta, byInputBuffer, 2, &dwNumBytesRead);
  printf("Number of bytes read from EEPROM: %x\n", dwNumBytesRead);
  ByteDataRead = byInputBuffer[0]; 
  printf(" ByteDataRead %02X\n",ByteDataRead);
  byOutputBuffer[dwNumBytesToSend++] = 0x80;
  byOutputBuffer[dwNumBytesToSend++] = 0x02;
  byOutputBuffer[dwNumBytesToSend++] = 0x13;
  HighSpeedSetI2CStop();
  ftStatus = FT_Write(ftHandle_fta, byOutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
  dwNumBytesToSend = 0;
  return 0;
}

char * timeheader(){
  curt_=time(NULL);
  loct_=localtime(&curt_);
  sprintf(thistime,"M%02dD%02dY%04dH%02d:%02d",loct_->tm_mon+1,loct_->tm_mday,loct_->tm_year+1900,loct_->tm_hour,loct_->tm_min);
  return thistime;
}

