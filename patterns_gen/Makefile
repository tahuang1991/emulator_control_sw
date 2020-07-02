SHELL := /bin/bash


# default compiler settings
CC = g++
OPT = -O3 -std=c++0x
LDFLAGS = -lm

# set pattern conversion name
Convert_EXE = pattern_convert.exe
Convert_SRC = pattern_convert.cc
Convert_OBJ = pattern_convert.o

#set pattern generation
Gen_EXE = pattern_generation_tmbb.exe
Gen_SRC = PatternGen.cc CSCConstants.h
Gen_OBJ = pattern_generation.o

#set pattern generation
Genhits_EXE = pattern_generationhits_tmbc.exe
Genhits_SRC = PatternGenhits.cc CSCConstants.h
Genhits_OBJ = pattern_generationhits.o

#set pattern generation
YuriyGen_EXE = Yuriy_CLCT_PatternGen_tmbc.exe
YuriyGen_SRC = Yuriy_CLCT_PatternGen.cc CSCConstants.h
YuriyGen_OBJ = Yuriy_CLCT_PatternGen.o

#set pattern generation
GEM_EXE = GEMpattern_generation.exe
GEM_SRC = GEMPatternGen.cc CSCConstants.h
GEM_OBJ = GEMpattern_generation.o

#set
GEMCluster_EXE = GEMClusters_generation.exe
GEMCluster_SRC = GEMClustersGen.cc CSCConstants.h
GEMCluseter_OBJ = GEMClusters_generation.o

# compilation for runs
all: convert generate generatehits GEMGen GEMClusterGen YuriyGen
 convert:
	$(CC) -g $(OPT) $(Convert_SRC) -o $(Convert_EXE) $(LDFLAGS)
	/bin/rm -rf *.o

 generate:
	$(CC) -g $(OPT) $(Gen_SRC) -o $(Gen_EXE) $(LDFLAGS)
	/bin/rm -rf *.o

 generatehits:
	$(CC) -g $(OPT) $(Genhits_SRC) -o $(Genhits_EXE) $(LDFLAGS)
	/bin/rm -rf *.o

 GEMGen:
	$(CC) -g $(OPT) $(GEM_SRC) -o $(GEM_EXE) $(LDFLAGS)
	/bin/rm -rf *.o
 GEMClusterGen:
	$(CC) -g $(OPT) $(GEMCluster_SRC) -o $(GEMCluster_EXE) $(LDFLAGS)
	/bin/rm -rf *.o
 YuriyGen:
	$(CC) -g $(OPT) $(YuriyGen_SRC) -o $(YuriyGen_EXE) $(LDFLAGS)
	/bin/rm -rf *.o


clean:
	rm -rf *.o $(Convert_EXE) $(Gen_EXE) $(GEM_EXE) $(Genhits_EXE)
