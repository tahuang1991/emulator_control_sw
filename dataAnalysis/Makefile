SHELL := /bin/bash


# default compiler settings
CC = g++
OPT = -O3 -std=c++0x
LDFLAGS=-Wl,--no-as-needed
LDFLAGS+=`root-config --ldflags`
#CXXFLAGS=-pedantic -ansi -Wall -Wno-long-long -Wno-format -Werror=uninitialized -Werror=delete-non-virtual-dtor -O2
CXXFLAGS=`root-config --cflags`
#CXXFLAGS+=`root-config --cflags`
ROOTLIB=$(shell root-config --glibs 2> /dev/null)
ROOTINCLUDE=-I$(shell root-config --incdir)
#test: 
#	echo "ROOT include  "$(ROOTINCLUDE)

	
# set pattern conversion name
CLCTAnalyse_EXE = CLCTAnalyse.exe
CLCTAnalyse_SRC = CLCTAnalyse.cc
CLCTAnalyse_OBJ = CLCTAnalyse.o

all: test CLCTAnalyse
 test:
 CLCTAnalyse:
	$(CC) $(CXXFLAGS) -g $(OPT) $(CLCTAnalyse_SRC) $(ROOTLIB) -o $(CLCTAnalyse_EXE) $(LDFLAGS)
	/bin/rm -rf *.o

clean:
	rm -rf *.o $(CLCTAnalyse_EXE)
