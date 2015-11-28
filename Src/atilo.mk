include ../common.mk

OBJS = atilo.o config.o endgame.o game.o layout.o loadsave.o menu.o play.o ponder.o settings.o \
	status.o util.o 
TARGET = atilo
LIBS = 
CFLAGS +=
#ifeq "$(DEBUG)" "1"
#	CFLAGS += -g -O0
#else
#	CFLAGS += -g0 -O2
#endif
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
