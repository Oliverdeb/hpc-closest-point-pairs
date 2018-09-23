
GCC=g++
FLAGS=-std=c++14 -o main

clean: 
	@ [ -e main ] && rm main || echo 'could not rm';

default: clean
	$(GCC) $(FLAGS) *.cpp

run: default
	./main -i inp.txt -o s