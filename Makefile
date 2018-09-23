
GCC=g++
FLAGS=-std=c++11 -o main

clean: 
	@ [ -e main ] && rm main || echo 1;

default: clean
	$(GCC) $(FLAGS) *.cpp

run: default
	./main