CC = gcc
CPP = g++
CFLAGS = -g -Wall
#CPPFLAGS = C preprocessor flags
#CXXFLAGS = C++ flags
#LDFLAGS = linker flags
#LDLIBS = -lmodbus
SOURCES = lab2A.c read_cf.c read_kb.c lab2Extra.c
OBJS = $(SOURCES:.c=.o)
DEPS = $(OBJS:.o=.d)
vpath %.c $dir $(MAKEFILE_LIST))
CPPFLAGS += -MMD -MP
all: lab2A read_cf read_kb lab2Extra
lab2A: lab2A.o InputGenDebian.o
read_cf: read_cf.o InputGenDebian.o
read_kb: read_kb.o
lab2Extra: lab2Extra.o
.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS) mtest core *.gdb
-include $(DEPS)
