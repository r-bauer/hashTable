# makefile for hash table using external chainning 

IDIR=./include
SDIR=./src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=./lib

LIBS=-lm

_DEPS=	hashtab.h \
	llapp.h \
	llgen.h \
	main.h
DEPS=$(addprefix $(IDIR)/, $(_DEPS))

_OBJ=	main.o \
	llapp.o \
	llgen.o \
	hashtab.o 
#OBJ=$(patsubst %, $(ODIR)/%, $(_OBJ))
OBJ=$(addprefix $(ODIR)/, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hashTab: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# https://www.gnu.org/software/make/manual/make.html#Prerequisite-Types
all: $(OBJ)

$(OBJ): | $(ODIR)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY:	clean

clean: 
	rm -fr $(ODIR) hashTab* *~ $(IDIR)/*~ $(SDIR)/*~

