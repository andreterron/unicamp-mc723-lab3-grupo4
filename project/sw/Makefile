# ####################################################
# Hello World
# ####################################################
CC = mips-elf-gcc
CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm

LIBS := uos pthread sha
SRCS := main.c sha_driver.c
OBJS := $(SRCS:.c=.o)

TARGET = sha_driver.mips
INC_DIR := -I. $(foreach lib, $(LIBS), -I./$(lib))
LIB_DIR := $(foreach lib, $(LIBS), -L./$(lib))
LDFLAGS = -lm $(foreach lib, $(LIBS), -l$(lib))

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

all:
	for lib in $(LIBS); do echo "   => Making" $$lib ...;\
		cd $$lib; $(MAKE); cd ..; done
	echo "   => Making exe ..."
	$(MAKE) build_app

$(LIBS):
	@make -C $@

build_app: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(LIB_DIR) -o $(TARGET) -Wl,--start-group $(LDFLAGS) $(OBJS) -Wl,--end-group

#------------------------------------------------------
clean:
	for lib in $(LIBS); do echo "  => Making" $$lib ...; \
		cd $$lib; $(MAKE) clean; cd ..; done
	rm -f *.o *~ $(TARGET)
#------------------------------------------------------
distclean: clean
#------------------------------------------------------
.c.o:
	$(CC) -c $(CFLAGS) $(INC_DIR) -c $<
	



