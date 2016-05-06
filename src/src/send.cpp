#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

using namespace std;

int main(int argc,char *argv[])
{
    std::cout <<" send cpp on cmslab1 " << std::endl;
	int pageid = atoi(argv[1]);
	cout << "pageid = " << pageid << endl;

	char block[RAMPAGE_SIZE];
	FILE* infile = fopen(argv[2],"r");
	fread(block, sizeof(char), RAMPAGE_SIZE, infile);
	memcpy(wdat,block,RAMPAGE_SIZE);
	cout << "block = " << block << endl;

	//nwdat = 4+RAMPAGE_SIZE;

	// open device
	eth_open("/dev/schar4");
	eth_reset();

	int e = write_command(7,pageid, block);
	cout << "write command 7 status = " << e << endl;

	char* pkt;
	e = read_command(&pkt);
	cout << "read command 7 status = " << e << endl;

        char tmp[2];
        tmp[0]=pageid&0x00ff;
        tmp[1]=(pageid&0xff00)>>8;
        std::cout<<"DEBUG[commands.cpp]  send_RAMPage, compare address bytes:"<<std::endl;
        std::cout<<HEX(tmp[0])<<" "<<HEX(pkt[2]&0xff)<<std::endl;
        std::cout<<HEX(tmp[1])<<" "<<HEX(pkt[3]&0xff - 0xd0)<<std::endl;
        std::cout<<DEC();

	eth_close();

  	return e;
}
