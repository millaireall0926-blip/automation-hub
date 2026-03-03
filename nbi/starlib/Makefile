target=com

PKGNAME=starlib
OBJDIR = ../tmp/$(target)/$(PKGNAME)
BINPREFIX = 
DEST = ../abin

.SILENT: 

CC = $(BINPREFIX)g++
STRIP = $(BINPREFIX)strip
INCLUDES = $(ARMINCLUDES)
LIBS = $(ARMLIBS)
CFLAGS = $(ARMCFLAGS) -Wall -O2 -DNDEBUG -fPIC -Wno-deprecated -fpermissive\
	-fomit-frame-pointer -fno-strict-aliasing -lgcc_s \
	-Wunused-result -Wno-write-strings 

LFLAGS = $(ARMLFLAGS) -shared -fPIC

COMPILE = $(CC) $(INCLUDES) $(CFLAGS)
LINK = $(CC) $(CFLAGS)

.SUFFIXES: .cpp .o

LOBJS = \
	$(OBJDIR)/function.o \
	$(OBJDIR)/starlib.o \
	$(OBJDIR)/cs_string.o \
	$(OBJDIR)/tinystr.o \
	$(OBJDIR)/tinyxml.o \
	$(OBJDIR)/tinyxmlerror.o \
	$(OBJDIR)/tinyxmlparser.o \
	$(OBJDIR)/star_session.o \
	$(OBJDIR)/star_socket.o \
	$(OBJDIR)/star_client.o 
	

LIB_HDR = compat.h


BIN = $(DEST)/$(PKGNAME).so

.cpp.o:
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

all: check_dir $(BIN) 

check_dir:
	[ ! -f $OBJDIR ] && mkdir -p $(OBJDIR)
	
clean:
	rm -fr $(OBJDIR) $(DEST)/$(PKGNAME).so

$(DEST)/$(PKGNAME).so: $(LOBJS)
	@echo link $@
	$(LINK) $(LFLAGS) $(LOBJS) $(LIBS) -o $@
	$(STRIP) $@

$(OBJDIR)/starlib.o: starlib.cpp function.h star_client.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/function.o: function.cpp function.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/cs_string.o: cs_string.cpp cs_basic.h cs_string.h cs_list.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/tinystr.o: tinystr.cpp tinyxml.h tinystr.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/tinyxml.o: tinyxml.cpp tinyxml.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/tinyxmlerror.o: tinyxmlerror.cpp tinyxml.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/tinyxmlparser.o: tinyxmlparser.cpp tinyxml.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/star_socket.o: star_socket.cpp star_socket.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/star_session.o: star_session.cpp star_session.h star_socket.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/star_client.o: star_client.cpp star_client.h star_session.h $(LIB_HDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<
