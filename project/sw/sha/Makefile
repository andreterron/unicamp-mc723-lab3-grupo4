# By default, the code is compiled for a "big endian" machine.
# To compile on a "little endian" machine set the LITTLE_ENDIAN flag.
# To make smaller object code, but run a little slower, don't use UNROLL_LOOPS.
# To use NIST's modified SHA of 7/11/94, define USE_MODIFIED_SHA

CC = mips-elf-gcc
AR = mips-elf-ar
CFLAGS = -msoft-float -specs=archc

TARGET = sha
INC_DIR := -I.

SRCS := sha.c
OBJS := $(SRCS:.c=.o)

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

#------------------------------------------------------
all: $(OBJS)
	$(AR) -r lib$(TARGET).a $(OBJS)
#------------------------------------------------------
clean:
	rm -f *.o *~ lib$(TARGET).a sha out*
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	

