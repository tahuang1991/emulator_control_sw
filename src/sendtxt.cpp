#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"
#include "patternhelpers.h"

using namespace std;

int main(int argc,char *argv[])
{
    std::cout <<" send cpp on cmslab1 " << std::endl;

    if(argc <= 1) {
        std::cout << "Error! No filename specified!" << std::endl;
        return -1;
    }else {
        std::ifstream infile(argv[1]);
        if (not(infile.good())){
            std::cout <<" Error! Failed to find or open input txt file " << std::endl;
            return -1;
        }
    }
    
	char block[CSCConstants::NUM_DCFEBS][RAMPAGE_SIZE];
    std::fstream text_file(argv[1], std::ios_base::in);
	// open device
    eth_open("/dev/schar3");
    eth_reset();
    int numfebs = convert(text_file, block);
    if(numfebs <= 0){
        std::cout <<" Error! number of dcfebs is less than 0 " << std::endl;
        return -1;
    }
    int e=-1;
    for (int i=0; i<numfebs; i++){
	   memcpy(wdat,block[i],RAMPAGE_SIZE);
	   //cout << "block = " << block[i] << endl;

	//nwdat = 4+RAMPAGE_SIZE;

	   e = write_command(7, i, block[i]);// here should be i or i+1???
	   cout << "write command 7 status = " << e <<" pageid "<< i+1 << endl;

	   char* pkt;
	   e = read_command(&pkt);
	   //cout << "read command 7 status = " << e << endl;

    }
	eth_close();

  	return e;
}
