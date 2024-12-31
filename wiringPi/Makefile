CXX = g++
CXXSTANDARD = -std=c++20
OPTFLAGS = -O3 -flto -fwhole-program -fconstexpr-depth=1024
MFLAGS = -march=native -mtune=native
WFLAGS = -Wall -Wextra -Wpedantic -Werror -Wattributes -Wbuiltin-macro-redefined \
-Wcast-align -Wconversion -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal \
-Wformat-security -Wformat=2 -Wimplicit-fallthrough=5 -Winline -Wint-to-pointer-cast \
-Wlogical-op -Woverflow -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion \
-Wsign-promo -Wstrict-aliasing=3 -Wstringop-overflow=4 -Wwrite-strings
# EXTRA_CXXFLAGS = -DWIRINGPI_DEBUG
# EXTRA_CXXFLAGS = -DCOMPILE_WIRINGPI_TESTS
# EXTRA_CXXFLAGS = -DWIRINGPI_DEBUG -DCOMPILE_WIRINGPI_TESTS

CXXFLAGS = $(CXXSTANDARD) $(OPTFLAGS) $(MFLAGS) $(WFLAGS) -pipe -fPIC $(EXTRA_CXXFLAGS)

default:
	make clean
	$(CXX) $(CXXFLAGS) wiringPi.C -o test

clean:
	rm -rf test