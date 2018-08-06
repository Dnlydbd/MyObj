#
# generate source file list
#

prjPath = ../

VPATH += ../init 

SOURCES = $(foreach dir,$(VPATH),$(wildcard $(dir)/*))

C_SRCS   =  rbtree.c rbtree_test.c 
CPP_SRCS = $(filter %.cpp,$(SOURCES))

C_OBJS   = $(C_SRCS:%.c=%.o)
CPP_OBJS = $(CPP_SRCS:%.cpp=%.o)

INCLUDE_DIR = -I$(prjPath)include
GLOBAL_FLAG = -g

GCPP = g++
CC	= gcc
AR = ar
RANLIB = ranlib

CFLAGS = $(GLOBAL_FLAG)
INC_DIR += $(INCLUDE_DIR)

TARGET = memtest

all : $(TARGET) 

$(TARGET) : $(C_OBJS) $(CPP_OBJS)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $^ -o $@
%.o:%.cpp
	$(GCPP) $(CFLAGS) $(INC_DIR) -c $^ -o $@	

clean :
	rm -f $(TARGET) $(C_OBJS) $(CPP_OBJS)


