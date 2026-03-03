target=com

PKGNAME=stardaemon
LIBNAME=starlib
OBJDIR = ../tmp/$(target)/$(PKGNAME)
BINPREFIX = 
DEST = ../abin


.SILENT: 

CC = $(BINPREFIX)g++
STRIP = $(BINPREFIX)strip
INCLUDES = $(ARMINCLUDES)
LIBS = $(ARMLIBS)
CFLAGS = $(ARMCFLAGS) -Wall -O2 -DNDEBUG -fPIC -Wno-deprecated -fpermissive -fomit-frame-pointer -fno-strict-aliasing -lgcc_s \
-L/star/eapi -Wl,--no-as-needed -rdynamic -ldl -lEAPI_Library -lI2CReset


LFLAGS = $(ARMLFLAGS) -shared -fPIC


COMPILE = $(CC) $(INCLUDES) $(CFLAGS)
LINK = $(CC) $(CFLAGS)

.SUFFIXES: .cpp .o

STAR_HDR = compat.h proc.h star_server.h star_session.h star_socket.h

DOBJS = \
	$(OBJDIR)/stardaemon.o \
	$(OBJDIR)/star_socket.o \
	$(OBJDIR)/star_session.o \
	$(OBJDIR)/star_server.o \
	$(OBJDIR)/proc.o \
	$(OBJDIR)/ioc.o \
	$(OBJDIR)/eapi_function.o \
	$(OBJDIR)/ec.o \
	$(OBJDIR)/eeprom.o 
	

BIN = $(DEST)/stardaemon

.cpp.o:
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

all: check_dir $(BIN) 

#check_dir:
#	[ ! -f $OBJDIR ] && mkdir -p $(OBJDIR)
#	[ "$target" != "IX86" ] && mkdir -p ../tmp/IX86/bin
check_dir:

clean:
	rm -fr $(OBJDIR)/*.o $(DEST)/stardaemon

$(DEST)/stardaemon: $(DEST)/$(LIBNAME).so $(DOBJS)
	@echo link $@ 
	$(LINK) -Wl,--no-as-needed -ldl $(DOBJS) -o $@
	$(STRIP) $@

$(OBJDIR)/star_socket.o: star_socket.cpp star_socket.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/star_session.o: star_session.cpp star_session.h star_socket.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/star_server.o: star_server.cpp star_server.h star_session.h star_socket.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/proc.o: proc.cpp proc.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/ioc.o: ../starAPI/ioc.cpp ../starAPI/ioc.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/stardaemon.o: stardaemon.cpp ../starlib/starlib_dif.h star_server.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/eeprom.o: ../starAPI/eeprom.cpp ../starAPI/eeprom.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/eapi_function.o: ../starAPI/eapi/eapi_function.cpp ../starAPI/eapi/eapi_function.h $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/ec.o: ../starAPI/eapi/ec.cpp ../starAPI/eapi/ec.cpp $(STAR_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

