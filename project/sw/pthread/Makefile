CC = mips-elf-gcc
AR = mips-elf-ar
CFLAGS = -msoft-float -specs=archc

TARGET = pthread
INC_DIR := -I. -I../uos

SRCS := pthread.c
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
	rm -f *.o *~ lib$(TARGET).a
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	



