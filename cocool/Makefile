
CC ?= gcc
GC ?= g++

TARGET = tomato

INC_DIR := -I../include
SRC_DIR := .
VPATH = $(SRC_DIR)

CFLAGS = -Wall -O -g
SOURCES := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp)
OBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@
	#cp -f $@ $(LIB_DIR)
	#$(AR) -cr $@ $(OBJS)
	
%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

%.o:%.cpp
	$(GC) $(CFLAGS) $(INC_DIR) -c $< -o $@
	
.PHONY : clean
clean:
	rm -rf $(OBJS) $(TARGET)
