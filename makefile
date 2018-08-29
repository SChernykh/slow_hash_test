all: *.h common/*.h *.c *.cpp
	gcc-8 -c -O3 -msse2 -maes -fsanitize=undefined -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize-address-use-after-scope *.c
	g++-8 -c -O3 -fsanitize=undefined -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize-address-use-after-scope main.cpp
	g++-8 -o slow_hash *.o -lasan -lubsan
