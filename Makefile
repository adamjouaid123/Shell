.PHONY: all clean

CC=gcc
CFLAGS=-Wall -g
LIBS+=-lpthread

INCLUDE = src/readcmd.h src/csapp.h src/commands.h src/jobs.h src/command_utils.h src/command_handlers.h src/jobs_utils.h src/signal_handlers.h src/error_handlers.h
SRCDIR = src

SRCS = $(wildcard $(SRCDIR)/*.c)

OBJS = $(SRCS:.c=.o)

INCLDIR = -I$(SRCDIR)

all: shell clean_objs

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $<

shell: $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

clean_objs:
	rm -f $(SRCDIR)/*.o

clean:
	rm -f shell
