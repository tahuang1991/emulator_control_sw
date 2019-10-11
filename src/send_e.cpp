#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "eth_lib.h"
#include "utils.h"
#include "commands.h"

using namespace std;

int main(int argc,char *argv[])
{
    int n = 1;
    if(argc > 1) {
        n = atoi(argv[1]);
    }

	eth_open("/dev/schar3");
	eth_reset();

	int e = write_command(0xe,n);
	cout << "write command e status = " << e << endl;

	char* pkt;
	e = read_command(&pkt);
	cout << "read command e status = " << e << endl;
	
	dumphex(e, pkt, & std::cout);

	eth_close();

  	return e;
}
