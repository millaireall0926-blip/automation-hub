PKGNAME=romwrite

CC = g++
DEST = ./
STARLIB = $(LIBPATH)starapi.a
CFLAGS = -fno-operator-names 
LIBS = -L/star/eapi -Wl,--no-as-needed -rdynamic -ldl -lEAPI_Library -lI2CReset -pthread

LIBPATH = /star/StarAPI/

BIN = $(PKGNAME)

all: $(BIN)
	@echo compile for $(PKGNAME)
	
#.SILENT: 

clean:	
	rm -fr $(BIN) *.o

$(BIN) : $(PKGNAME).cpp
	$(CC) $(CFLAGS) -o $@ $< -I$(LIBPATH) -L./ $(STARLIB) $(LIBS)    
    





	 
