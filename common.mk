CROSS_COMPILE ?= 
CONFIG_NAME ?= Debug

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

vpath %.c   ../Src/:./
vpath %.h   ../Src/:./
vpath %.o   ./

CFLAGS = -Wall -std=gnu99 
CFLAGS += -I ../Src
CFLAGS += `pkg-config  gtk+-2.0 --cflags`
LDFLAGS += `pkg-config gtk+-2.0 --libs` -l X11

%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.c
	@$(RM) $@
	$(CC) $(CFLAGS) -o $@ -c $< 
