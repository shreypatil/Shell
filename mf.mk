
IDIR=include
SDIR=src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm 

_DEPS = procll.h cd.h colors.h echo.h execute.h gvars.h libs.h ls.h pinfo.h printwd.h bg.h fg.h jobs.h sig.h sighan.h replay.h myutils.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = shell.o procll.o cd.o colors.o echo.o execute.o ls.o pinfo.o printwd.o bg.o fg.o jobs.o sig.o sighan.o replay.o myutils.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 