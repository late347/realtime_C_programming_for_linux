CC = gcc
CPP = g++
CFLAGS = -g -Wall
#CPPFLAGS = C preprocessor flags
#CXXFLAGS = C++ flags
#LDFLAGS = linker flags
#LDLIBS = -lmodbus
SOURCES = lab1.c lab2.c
OBJS = $(SOURCES:.c=.o)
DEPS = $(OBJS:.o=.d)
vpath %.c $dir $(MAKEFILE_LIST))
CPPFLAGS += -MMD -MP
all: lab1 lab2
lab1: lab1.o
lab2: lab2.o
.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS) mtest core *.gdb
-include $(DEPS)
