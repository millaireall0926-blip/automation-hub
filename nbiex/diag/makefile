SOURCE=diag
FOLDER=diag
CC=g++
STARLIB = $(LIBPATH)starapi.a
CFLAGS = -fno-operator-names 
LIBS = -ldl -pthread
LIBPATH = /mnt/d/nbiex/abin/
HEADPATH = /mnt/d/nbiex/StarAPI/
BIN = $(FOLDER).pgm

all: $(PATBIN) $(BIN)
	@echo Compiled PGM:$(FOLDER)

.SILENT: 
clean:	
	rm -fr $(BIN) *.o

$(BIN) : $(SOURCE).cpp
	$(CC) $(CFLAGS) -o $@ $< -I$(HEADPATH) -L./ $(STARLIB) $(LIBS)
