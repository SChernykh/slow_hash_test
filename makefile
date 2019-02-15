all: *.h common/*.h *.c *.cpp
	gcc -c CryptonightR_template.S -o CryptonightR_template.o
	gcc -c -O3 -msse2 -maes *.c
	g++ -c -O3 -std=c++11 main.cpp
	g++ -o slow_hash *.o
