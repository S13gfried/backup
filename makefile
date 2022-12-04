EXENAME=backup
COMPL=gcc
CFLAGS=
SOURCES=backup.c

all:
	$(COMPL) $(CFLAGS) $(SOURCES) -o $(EXENAME)