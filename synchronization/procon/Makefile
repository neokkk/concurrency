obj-m += procon_kernel.o

kernel_version := $(shell uname -r)

ARCH=arm64
CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-gcc-
KERNEL_DIR ?= /lib/modules/$(kernel_version)/build

CC=aarch64-linux-gnu-gcc

OBJS = procon_user.o
CFLAGS=
TARGET=target

all default: modules target
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) $@

target: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<