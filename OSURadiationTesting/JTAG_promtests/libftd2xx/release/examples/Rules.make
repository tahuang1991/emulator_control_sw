CC=gcc
INC_DIR := $(shell cd ..; pwd)
CFLAGS=-Wall -Wextra -L. -lftd2xx -Wl,-rpath -I$(INC_DIR)
 
