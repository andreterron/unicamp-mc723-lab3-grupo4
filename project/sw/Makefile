# ####################################################
# Hello World
# ####################################################

CC = mips-elf-gcc
CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm

SUB_FOLDERS = sha
TARGET = hello_world.mips
INC_DIR := -I.

SRCS := hello_world.c lib_pthread.c
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	for a in $(SUB_FOLDERS); do echo " => Making" $$a ...; \
	    cd $$a; $(MAKE); cd ..; done
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET)
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	



