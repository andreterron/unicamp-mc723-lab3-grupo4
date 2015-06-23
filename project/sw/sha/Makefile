# By default, the code is compiled for a "big endian" machine.
# To compile on a "little endian" machine set the LITTLE_ENDIAN flag.
# To make smaller object code, but run a little slower, don't use UNROLL_LOOPS.
# To use NIST's modified SHA of 7/11/94, define USE_MODIFIED_SHA

CC = mips-elf-gcc
CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm

TARGET = sha.mips
INC_DIR := -I.

SRCS := sha.c sha_driver.c ../lib_pthread.c
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
#------------------------------------------------------
clean:
	rm -f *.o *~ $(TARGET) sha out*
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	

