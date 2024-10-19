CXX = g++
CXXSTANDARD = -std=c++20
OPTFLAGS = -O3 -flto -fconstexpr-depth=1024
MFLAGS = -march=native -mtune=native
WFLAGS = -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3

CXXFLAGS = $(OPTFLAGS) $(CXXSTANDARD) $(MFLAGS) $(WFLAGS) -pipe -fPIC $(EXTRA_CFLAGS)

default:
	make clean
	$(CXX) $(CXXFLAGS) wiringPi.C -o test

clean:
	rm -rf test