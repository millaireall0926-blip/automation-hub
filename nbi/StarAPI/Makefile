target=com
PKGNAME=starapi
OBJDIR = ../tmp/$(target)/$(PKGNAME)
BINPREFIX =
ARMCFLAGS = -DCU50
DEST = ../abin
STARPATH = /mnt/c/'Program Files (x86)'/StarTest-NBI/CM/

CC = $(BINPREFIX)g++
AR = $(BINPREFIX)ar
STRIP = $(BINPREFIX)strip
INCLUDES = $(ARMINCLUDES)
CFLAGS =  $(ARMCFLAGS)  -std=c++0x -Wall -fPIC -fno-operator-names \
-Wno-write-strings -Wno-unused-result -Wno-unused-variable -Wno-format -Wno-conversion-null -Wno-return-type \
-Wno-psabi \
-L/star/eapi -Wl,--no-as-needed -rdynamic -ldl -lEAPI_Library -lI2CReset

LFLAGS = $(ARMLFLAGS) -fPIC -rdynamic

COMPILE = $(CC) $(INCLUDES) $(CFLAGS)
LINK = $(AR) 



.SUFFIXES: .cpp .o

LOBJS = \
	$(OBJDIR)/ioc.o \
	$(OBJDIR)/net.o \
	$(OBJDIR)/tgc.o \
	$(OBJDIR)/STAR_TEST.o \
	$(OBJDIR)/starAPI.o \
	$(OBJDIR)/Scanner.o \
	$(OBJDIR)/result.o \
	$(OBJDIR)/pgc.o \
	$(OBJDIR)/failmem.o \
	$(OBJDIR)/errc.o \
	$(OBJDIR)/bib.o	\
	$(OBJDIR)/cmd.o \
	$(OBJDIR)/operation.o \
	$(OBJDIR)/DIAG_TEST.o \
	$(OBJDIR)/dps.o \
	$(OBJDIR)/vec.o \
	$(OBJDIR)/nand.o \
	$(OBJDIR)/eeprom.o \
	$(OBJDIR)/eapi_function.o \
	$(OBJDIR)/ec.o 


LHDR = tgc.h String.h ../starlib/starlib_dif.h STAR_TEST.h define.h starAPI.h \
Scanner.h result.h pgc.h net.h macro.h ioc.h failmem.h errc.h DIAG_TEST.h cmd.h operation.h \
bib.h eapi/eapi_function.h eapi/ec.h eeprom.h dps.h bitcharts.h vec.h nand.h

COPYFILE = STAR_TEST.h DIAG_TEST.h macro.h

BIN = $(DEST)/$(PKGNAME).a

.SILENT: 

.cpp.o:
	@echo compile for $(target) : $<
	$(COMPILE) -c -o $@ $<

all:	check_dir $(BIN)


check_dir:
	[ ! -f $OBJDIR ] && mkdir -p $(OBJDIR)
clean:
	rm -fr $(BIN) $(OBJDIR)/*.o

$(DEST)/$(PKGNAME).a: $(LOBJS)
	@echo link $@
#	@echo copy $(LIBCP)/$(PKGNAME).a
	@echo copy $(COPYFILE) $(BIN) to $(STARPATH)
	$(LINK) rcs $@ $(LOBJS)
	#cp $@ $(LIBCP)
	cp $(COPYFILE) $(STARPATH) 
	cp  $(BIN) $(STARPATH) 


$(OBJDIR)/ioc.o: ioc.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/net.o: net.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/tgc.o: tgc.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/STAR_TEST.o: STAR_TEST.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/starAPI.o: starAPI.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/Scanner.o: Scanner.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/result.o: result.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/pgc.o: pgc.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/failmem.o: failmem.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/errc.o: errc.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/bib.o: bib.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/cmd.o: cmd.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/operation.o: operation.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/DIAG_TEST.o: DIAG_TEST.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/dps.o: dps.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/vec.o: vec.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/nand.o: nand.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/eeprom.o: eeprom.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/eapi_function.o: eapi/eapi_function.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<

$(OBJDIR)/ec.o: eapi/ec.cpp $(LHDR)
	@echo compile for $(target): $<
	$(COMPILE) -c -o $@ $<




