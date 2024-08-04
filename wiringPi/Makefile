#VERSION=$(shell cat ../VERSION)
#DESTDIR?=/usr
#PREFIX?=/local
#LDCONFIG?=ldconfig
#STATIC=libwiringPi.a
#DYNAMIC=libwiringPi.so.$(VERSION)

CXX = g++
CXXSTANDARD = -std=c++20
OPTFLAGS = -O3 -flto -fconstexpr-depth=1024
MFLAGS = -march=native -mtune=native
WFLAGS = -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3

#INCLUDE	= -I.
#DEFS = -D_GNU_SOURCE
CXXFLAGS = $(OPTFLAGS) $(CXXSTANDARD) $(MFLAGS) $(WFLAGS) $(INCLUDE) $(DEFS) -pipe -fPIC $(EXTRA_CFLAGS)
#LIBS = -lm -lpthread -lrt -lcrypt

default:
	$(CXX) $(CXXFLAGS) wiringPi.C -o test

#SRC	= wiringPi.C

#HEADERS = $(shell ls *.H)

#OBJ	= $(SRC:.C=.o)

#all: $(DYNAMIC)

#.PHONY:	static
#static:	
#	$Q cat noMoreStatic

#$(DYNAMIC):	$(OBJ)
#	$Q echo "[Link (Dynamic)]"
#	$(CXX) -shared -Wl,-soname,libwiringPi.so$(WIRINGPI_SONAME_SUFFIX) -o libwiringPi.so.$(VERSION) $(OBJ) $(LIBS)

#.C.o:
#	$Q echo [Compile] $<
#	$(CXX) -c $(CFLAGS) $< -o $@

#.PHONY:	clean
#clean:
#	$Q echo "[Clean]"
#	$Q rm -f $(OBJ) $(OBJ_I2C) *~ core tags Makefile.bak libwiringPi.*

#.PHONY:	tags
#tags:	$(SRC)
#	$Q echo [ctags]
#	$Q ctags $(SRC)

#.PHONY:	install
#install: $(DYNAMIC)
#	$Q echo "[Install Headers]"
#	$Q install -m 0755 -d $(DESTDIR)$(PREFIX)/include
#	$Q install -m 0644 $(HEADERS) $(DESTDIR)$(PREFIX)/include
#	$Q echo "[Install Dynamic Lib]"
#	$Q install -m 0755 -d $(DESTDIR)$(PREFIX)/lib
#	$Q install -m 0755 libwiringPi.so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/libwiringPi.so.$(VERSION)
#	$Q ln -sf $(DESTDIR)$(PREFIX)/lib/libwiringPi.so.$(VERSION)	$(DESTDIR)/lib/libwiringPi.so
#	$Q $(LDCONFIG)

#.PHONY:	uninstall
#uninstall:
#	$Q echo "[UnInstall]"
#	$Q cd $(DESTDIR)$(PREFIX)/include/ && rm -f $(HEADERS)
#	$Q cd $(DESTDIR)$(PREFIX)/lib/ && rm -f libwiringPi.*
#	$Q $(LDCONFIG)

#.PHONY:	depend
#depend:
#	makedepend -Y $(SRC) $(SRC_I2C)

# DO NOT DELETE

# wiringPi.o: softPwm.H softTone.H wiringPi.H ../version.H
#wiringPi.o: wiringPi.H
#wiringSerial.o: wiringSerial.H
#wiringShift.o: wiringPi.H wiringShift.H
#piHiPri.o: wiringPi.H
#piThread.o: wiringPi.H
#wiringPiSPI.o: wiringPi.H wiringPiSPI.H
#wiringPiI2C.o: wiringPi.H wiringPiI2C.H
#softPwm.o: wiringPi.H softPwm.H
#softTone.o: wiringPi.H softTone.H
#mcp23008.o: wiringPi.H wiringPiI2C.H mcp23x0817.H mcp23008.H
#mcp23016.o: wiringPi.H wiringPiI2C.H mcp23016.H mcp23016reg.H
#mcp23017.o: wiringPi.H wiringPiI2C.H mcp23x0817.H mcp23017.H
#mcp23s08.o: wiringPi.H wiringPiSPI.H mcp23x0817.H mcp23s08.H
#mcp23s17.o: wiringPi.H wiringPiSPI.H mcp23x0817.H mcp23s17.H
#sr595.o: wiringPi.H sr595.H
#pcf8574.o: wiringPi.H wiringPiI2C.H pcf8574.H
#pcf8591.o: wiringPi.H wiringPiI2C.H pcf8591.H
#mcp3002.o: wiringPi.H wiringPiSPI.H mcp3002.H
#mcp3004.o: wiringPi.H wiringPiSPI.H mcp3004.H
#mcp4802.o: wiringPi.H wiringPiSPI.H mcp4802.H
#mcp3422.o: wiringPi.H wiringPiI2C.H mcp3422.H
#max31855.o: wiringPi.H wiringPiSPI.H max31855.H
#max5322.o: wiringPi.H wiringPiSPI.H max5322.H
#ads1115.o: wiringPi.H wiringPiI2C.H ads1115.H
#sn3218.o: wiringPi.H wiringPiI2C.H sn3218.H
#bmp180.o: wiringPi.H wiringPiI2C.H bmp180.H
#htu21d.o: wiringPi.H wiringPiI2C.H htu21d.H
#ds18b20.o: wiringPi.H ds18b20.H
#drcSerial.o: wiringPi.H wiringSerial.H drcSerial.H
#pseudoPins.o: wiringPi.H pseudoPins.H
#wpiExtensions.o: wiringPi.H mcp23008.H mcp23016.H mcp23017.H mcp23s08.H
#wpiExtensions.o: mcp23s17.H sr595.H pcf8574.H pcf8591.H mcp3002.H mcp3004.H
#wpiExtensions.o: mcp4802.H mcp3422.H max31855.H max5322.H ads1115.H sn3218.H
#wpiExtensions.o: drcSerial.H pseudoPins.H bmp180.H htu21d.H ds18b20.H
#wpiExtensions.o: wpiExtensions.H
