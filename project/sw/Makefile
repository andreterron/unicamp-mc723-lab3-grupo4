# ####################################################
# Hello World
# ####################################################
CC = mips-elf-gcc
CFLAGS = -msoft-float -specs=archc
LDFLAGS = -lm

SUB_FOLDERS = sha

LIBS := uos pthread app
SRCS := main.c
OBJS := $(SRCS:.c=.o)

TARGET = demo_app.mips
INC_DIR := -I. $(foreach lib, $(LIBS), -I./$(lib))
LIB_DIR := $(foreach lib, $(LIBS), -L./$(lib))
LDFLAGS = -lm $(foreach lib, $(LIBS), -l$(lib))

#------------------------------------------------------
.SILENT:

#------------------------------------------------------
.SUFFIXES: .o .c .h

all:
	for a in $(SUB_FOLDERS); do echo " => Making" $$a ...; \
	    cd $$a; $(MAKE); cd ..; done
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
	



