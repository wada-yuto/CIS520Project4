# Compiles the OpenMP.code

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
	$(CC) -openmp -c -o $@ $< $(CFLAGS)

openmp: $(OBJ)
	$(CC) -fopenmp -openmp -o $@ $^ $(CFLAGS)
