# Compiles the pthread scorecard generator

INCDIR=../include
SRCDIR=../src

CC=gcc
CFLAGS=-I$(INCDIR)

OBJDIR=obj

_DEPS = pthread_scorecard_generator_v3.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = pthread_scorecard_generator_v3.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -pthread -c -o $@ $< $(CFLAGS)

pthread_scorecard_generator_v3: $(OBJ)
	$(CC) -pthread -o $@ $^ $(CFLAGS)
