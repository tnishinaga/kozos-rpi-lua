PREFIX  = /usr/
ARCH    = arm-none-eabi
BINDIR  = $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR      = $(BINDIR)/$(ADDNAME)ar
AS      = $(BINDIR)/$(ADDNAME)as
CC      = $(BINDIR)/$(ADDNAME)gcc
LD      = $(BINDIR)/$(ADDNAME)ld
NM      = $(BINDIR)/$(ADDNAME)nm
OBJCOPY = $(BINDIR)/$(ADDNAME)objcopy
OBJDUMP = $(BINDIR)/$(ADDNAME)objdump
RANLIB  = $(BINDIR)/$(ADDNAME)ranlib
STRIP   = $(BINDIR)/$(ADDNAME)strip

OBJS  = startup.o main.o interrupt.o vector.o interrupt_handler.o
OBJS += lib.o serial.o


# Lua objects
LUA_SRC=./lua-5.3.4/src/
LUA_CORE_O=	$(LUA_SRC)/lapi.o  \
	$(LUA_SRC)/lcode.o  \
	$(LUA_SRC)/lctype.o  \
	$(LUA_SRC)/ldebug.o  \
	$(LUA_SRC)/ldo.o  \
	$(LUA_SRC)/ldump.o  \
	$(LUA_SRC)/lfunc.o  \
	$(LUA_SRC)/lgc.o  \
	$(LUA_SRC)/llex.o  \
	$(LUA_SRC)/lmem.o  \
	$(LUA_SRC)/lobject.o  \
	$(LUA_SRC)/lopcodes.o  \
	$(LUA_SRC)/lparser.o  \
	$(LUA_SRC)/lstate.o  \
	$(LUA_SRC)/lstring.o  \
	$(LUA_SRC)/ltable.o  \
	$(LUA_SRC)/ltm.o  \
	$(LUA_SRC)/lundump.o  \
	$(LUA_SRC)/lvm.o  \
	$(LUA_SRC)/lzio.o
LUA_LIB_O= $(LUA_SRC)/lauxlib.o  \
	$(LUA_SRC)/lbaselib.o  \
	$(LUA_SRC)/lbitlib.o  \
	$(LUA_SRC)/lcorolib.o  \
	$(LUA_SRC)/ldblib.o  \
	$(LUA_SRC)/liolib.o  \
	$(LUA_SRC)/lmathlib.o  \
	$(LUA_SRC)/loslib.o  \
	$(LUA_SRC)/lstrlib.o  \
	$(LUA_SRC)/ltablib.o  \
	$(LUA_SRC)/lutf8lib.o  \
	$(LUA_SRC)/loadlib.o  \
	$(LUA_SRC)/linit.o
LUA_OBJ= $(LUA_CORE_O) $(LUA_LIB_O)

# sources of kozos
OBJS += kozos.o syscall.o memory.o consdrv.o command.o

TARGET = kozos

CFLAGS = -Wall -nostdinc -fno-builtin -std=gnu99
#CFLAGS += -mint32 # intを32ビットにすると掛算／割算ができなくなる
CFLAGS += -I.
CFLAGS += -g3
CFLAGS += -Os
CFLAGS += -march=armv6zk -mtune=arm1176jzf-s
CFLAGS += -DKOZOS

LFLAGS = -static -T ld.scr -L.
LFLAGS += -lm

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .S .o

all :		$(TARGET)

$(TARGET) : $(OBJS) lua
		$(CC) $(OBJS) $(LUA_OBJ) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

.c.o :		$<
		$(CC) -c $(CFLAGS) $<

.s.o :		$<
		$(CC) -c $(CFLAGS) $<

.S.o :		$<
		$(CC) -c $(CFLAGS) $<

.PHONY: clean lua
clean :
		rm -f $(OBJS) $(TARGET) $(TARGET).elf
		cd $(LUA_SRC) && make clean	

lua: 
	cd $(LUA_SRC) && make -j4 o
