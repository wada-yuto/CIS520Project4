# Compiles the pthread scorecard generator

INCDIR=../include
SRCDIR=../src

CC=gcc
CFLAGS=-I$(INCDIR)

OBJDIR=obj

_DEPS = pthread_scorecard_generator.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = pthread_scorecard_generator.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -pthread -c -o $@ $< $(CFLAGS)

pthread_scorecard_generator: $(OBJ)
	$(CC) -pthread -o $@ $^ $(CFLAGS)
