#
#  Makefile for the x86 version of the Xinu operating system
#

COMPILER_ROOT = ../arm-2013.11/bin/arm-none-eabi-

CC 	=	${COMPILER_ROOT}gcc
cPP	=	${COMPILER_ROOT}gcc -E
AS  	=	${COMPILER_ROOT}as
LD	=	${COMPILER_ROOT}ld
OBJCOPY	=	${COMPILER_ROOT}objcopy
LIB	= 	../lib/libxc
XINU	=	./xinu

MAKEDEP	= gcc -M -MG

DEFS	= -DBSDURG
CFLAGS_EXTRA =
INCLUDE = -I../include

# Amount to move loaded image down in memory

BRELOC  =	0x150000
BOOTPLOC=	0x150000

# Compiler flags
SDEFS	= -I../include
CFLAGS	= -mcpu=cortex-a8 -marm -fno-builtin -fno-stack-protector -nostdlib -c -Wall -O ${DEFS} ${INCLUDE} ${CFLAGS_EXTRA}

# Loader flags

LDFLAGS = -m armelf -L../arm-2013.11/lib/gcc/arm-none-eabi/4.8.1/

TOPDIR	= ..

# Components (files for each are listed below) 

COMPS	=	system 							\
		device/tty						\
		device/nam						\
		device/eth						\
		net 							\
		shell 							

# Start with empty source file list and add files for each directory
#

SRC_FILES =

#------------------------------------------------------------------------
# Files for ../system							#
#------------------------------------------------------------------------

SYSTEM_SFILES =								\
	start.S		ctxsw.S		clkint.S	intr.S

SYSTEM_CFILES =								\
	ascdate.c	bufinit.c	chprio.c	panic.c		\
	clkinit.c	close.c		conf.c		control.c	\
	create.c	freebuf.c	freemem.c	getbuf.c	\
	getc.c		getdev.c	getitem.c	getmem.c	\
	getpid.c	getprio.c 	getstk.c	init.c		\
	initialize.c 							\
			insert.c	insertd.c	ioerr.c		\
	ionull.c	kill.c		kprintf.c	main.c		\
	mkbufpool.c	newqueue.c	open.c				\
	ptclear.c	ptcount.c	ptcreate.c	ptdelete.c	\
	ptinit.c	ptrecv.c	ptreset.c	ptsend.c	\
	putc.c		queue.c		read.c		ready.c		\
	receive.c	recvclr.c	recvtime.c	resched.c	\
	resume.c	sched_cntl.c 	seek.c		semcount.c	\
	semcreate.c	semdelete.c	semreset.c	send.c		\
	signal.c	signaln.c	sleep.c		suspend.c	\
	unsleep.c	userret.c	wait.c		wakeup.c	\
	write.c		xdone.c		yield.c		evec.c		\
	gettime.c	getutime.c	

SYSTEM_SFULL = ${SYSTEM_SFILES:%=../system/%}
SYSTEM_CFULL = ${SYSTEM_CFILES:%=../system/%}

SRC_FILES += $(SYSTEM_SFULL)
SRC_FILES += $(SYSTEM_CFULL)

#------------------------------------------------------------------------
# Files for ../device/tty						#
#------------------------------------------------------------------------

#TTY_SFILES =								\
#	ttyDispatch.S


TTY_CFILES =								\
	ttyControl.c	ttyGetc.c	ttyInit.c	ttyInter_in.c	\
	ttyInter_out.c	ttyInterrupt.c	ttyKickOut.c	ttyPutc.c	\
	ttyRead.c	ttyWrite.c

#TTY_SFULL = ${TTY_SFILES:%=../device/tty/%}
TTY_CFULL = ${TTY_CFILES:%=../device/tty/%}

#SRC_FILES += $(TTY_SFULL)
SRC_FILES += $(TTY_CFULL)

#------------------------------------------------------------------------
# Files for ../device/nam                                                                                                                                                                        
#------------------------------------------------------------------------

NAM_CFILES =                                                            \
	mount.c         namInit.c       namOpen.c       nammap.c
	
NAM_CFULL  = ${NAM_CFILES:%=../device/nam/%}

SRC_FILES += $(NAM_CFULL)

#------------------------------------------------------------------------
# Files in directory ../device/eth
#------------------------------------------------------------------------

ETH_SFILES =								\
	ethDispatch.S

ETH_CFILES =								\
	ethInit.c 	ethInterrupt.c 	ethControl.c	ethRead.c 	\
	ethWrite.c 	e1000e_init.c 	e1000e_open.c 	e1000e_read.c 	\
	e1000e_write.c 	e1000e_cntl.c 	e1000e_intr.c 	e1000e_cmd.c 	\
	3c905C_init.c 	3c905C_open.c 	3c905C_read.c 	3c905C_write.c 	\
	3c905C_cntl.c 	3c905C_intr.c 	3c905C_cmd.c	e1000e_rar_set.c

ETH_SFULL = ${ETH_SFILES:%=../device/eth/%}
ETH_CFULL = ${ETH_CFILES:%=../device/eth/%}

#SRC_FILES += $(ETH_SFULL)
#SRC_FILES += $(ETH_CFULL)


#------------------------------------------------------------------------
# Files for ../net
#------------------------------------------------------------------------

NET_CFILES =								\
	net.c	 	arp.c 		udp.c 		pdump.c 	\
	dhcp.c		ip.c		icmp.c		nat.c		\
	netiface.c	netstart.c	dot2ip.c	hexdump.c

NET_CFULL  = ${NET_CFILES:%=../net/%}

SRC_FILES += $(NET_CFULL)

#------------------------------------------------------------------------
# Files for ../shell
#------------------------------------------------------------------------

SHELL_CFILES =								\
	addargs.c	lexan.c		shell.c

SHELL_CFILES +=								\
	xsh_argecho.c	xsh_cat.c	xsh_clear.c 	xsh_uptime.c	\
	xsh_echo.c	xsh_exit.c	xsh_devdump.c	xsh_help.c	\
	xsh_kill.c	xsh_led.c	xsh_memdump.c	xsh_ps.c 	\
	xsh_ping.c	xsh_sleep.c 	xsh_memstat.c	xsh_date.c	\
	xsh_arp.c	xsh_ipaddr.c 	xsh_udpecho.c	xsh_udpdump.c	\
	xsh_bingid.c	xsh_udpeserver.c

SHELL_CFULL  = ${SHELL_CFILES:%=../shell/%}

SRC_FILES += $(SHELL_CFULL)

#######################################
# Generate a list of all object files
#######################################

OBJ_TMP	= $(patsubst %.c,%.o,$(SRC_FILES)) 	# substitute .c => .o
OBJ_FILES  = $(patsubst %.S,%.o,$(OBJ_TMP)) 	# substitute .S => .o


# Export variables for recursive make calls (such as the library)
export

#------------------------------------------------------------------------
# make targets
#------------------------------------------------------------------------

all:	xinu

xinu: 	Makefile ../config/Configuration xinu.mapfile configure 	\
		$(OBJ_FILES) $(LIB)/libxc.a
		@echo;echo Now Loading all object files to produce GRUB bootable xinu: 
		@echo;echo ${OBJ_FILES} 
		$(LD) $(LDFLAGS) -dn -Ttext 0x81000000 -e start ${OBJ_FILES}	\
			${LIB}/libxc.a	-o ${XINU} -lgcc
		$(OBJCOPY) -O binary xinu xinu.bin
		mkimage -A arm -O linux -T kernel -C none -a 0x81000000 -e 0x81000000 -n 'Xinu' -d xinu.bin xinu.boot
		@echo

${LIB}/libxc.a:
		@(cd ${LIB}; make libxc.a)

objects: $(OBJ_FILES)

configure: ../include/conf.h 	../system/conf.c	
	
../include/conf.h ../system/conf.c: ../config/config ../config/Configuration
	@echo making conf.h and conf.c
	@(cd ../config; make install)

../config/config:
	make -C ../config/ -f ../config/Makefile clean all install
	
clean:
	@echo removing .o files in directories
	@echo $(COMPS)
	@rm -f ${OBJ_FILES} ${LIB}/*.a ${LIB}/*.o
	@echo removing lib ...
	@(cd ${LIB}; make clean)
	@echo   removing configuration files ...
	@rm -f ../include/conf.h ../system/conf.c
	@(cd ../config; make clean)
	@echo removing xinu ...
	@rm -f xinu


depend:
	@echo;echo Making all dependencies in .deps
	@$(MAKEDEP) ${INCLUDE} ${SRC_FILES} > .deps
	@echo;echo Finished making dependencies

depclean:
	rm -f .deps
	touch .deps

###################
# Special targets #
###################
${COMPS}:
	$(MAKE) COMPS="$@" objects

# Special rules for special files

../system/intr.o: ../system/intr.S
	${CPP} ${SDEFS} ../system/intr.S | ${AS} -o ../system/intr.o

../system/clkint.o: ../system/clkint.S
	${CPP} ${SDEFS} ../system/clkint.S | ${AS} -o ../system/clkint.o

../system/ctxsw.o: ../system/ctxsw.S
	${CPP} ${SDEFS} ../system/ctxsw.S | ${AS} -o ../system/ctxsw.o

../system/start.o: ../system/start.S Makefile
	${CPP} ${SDEFS} -DBRELOC=${BRELOC} -DBOOTPLOC=${BOOTPLOC} 	\
			../system/start.S | ${AS} -o ../system/start.o

../device/tty/ttyDispatch.o: ../device/tty/ttyDispatch.S
	${CPP} ${SDEFS} ../device/tty/ttyDispatch.S | ${AS} -o 		\
	../device/tty/ttyDispatch.o

../device/eth/ethDispatch.o: ../device/eth/ethDispatch.S Makefile
	${CPP} ${SDEFS} ../device/eth/ethDispatch.S | ${AS} -o 		\
	../device/eth/ethDispatch.o

head.o: head.S
	${CPP} ${SDEFS} head.S | ${AS} -o head.o

../system/initialize.o: ../system/initialize.c
	sh mkvers.sh
	${CC} -c ${CFLAGS} -DVERSION=\""`cat version`"\" 		\
		../system/initialize.c -o ../system/initialize.o

include .deps
