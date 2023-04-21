INCDIR=../include
SRCDIR=../src

CC=gcc
CFLAGS=-I$(INCDIR)

OBJDIR=obj

_DEPS = OpenMP.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = OpenMP.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

OpenMP: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
