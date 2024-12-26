CXX = g++
CXXSTANDARD = -std=c++20
OPTFLAGS = -O3 -flto -fconstexpr-depth=1024
MFLAGS = -march=native -mtune=native
WFLAGS = -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3

INCLUDE	= -I.
DEFS = -D_GNU_SOURCE
CXXFLAGS = $(OPTFLAGS) $(CXXSTANDARD) $(MFLAGS) $(WFLAGS) $(INCLUDE) $(DEFS) -pipe -fPIC $(EXTRA_CXXFLAGS)
LIBS = -lm -lpthread -lrt -lcrypt

default:
	make clean
	$(CXX) $(CXXFLAGS) readSystemInfo.C -o readSystemInfo
	sudo ./readSystemInfo
	mv piInfo.H ../

clean:
	rm -rf readSystemInfo
	rm -rf piInfo.H
	rm -rf ../piInfo.H