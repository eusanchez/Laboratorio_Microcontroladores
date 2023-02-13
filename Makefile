CFLAGS = -DTEST

OBJS = clock.o console.o

BINARY = spi-mems

LDSCRIPT = ../stm32f429i-discovery.ld

OOCD_INTERFACE = stlink-v2-1

include ../../Makefile.include
