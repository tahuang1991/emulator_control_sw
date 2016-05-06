#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

using namespace std;

int main(int argc,char *argv[])
{
	char mode = 0x0;
	if(argc > 1) {
		if(argv[1][0] == '0')
			mode = 0x0;
		else if(argv[1][0] == 'o')
			mode = 0x0;
		else if(argv[1][0] == 'c')
			mode - 0xc;
	}
	eth_open("/dev/schar4");
	eth_reset();

	int e = write_command(0xd);
	cout << "write command d status = " << e << endl;

	char* pkt;
	e = read_command(&pkt);
	cout << "read command d status = " << e << endl;
	
	dumphex(e, pkt, & std::cout);

	eth_close();

  	return e;
}
