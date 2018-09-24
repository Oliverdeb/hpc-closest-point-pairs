
GCC=g++
SRC=src/
BINDIR=bin/
INCLUDE=include/
FLAGS=-std=c++14 -I$(INCLUDE) -o $(BINDIR)main -Wall

default: clean
	$(GCC) $(FLAGS) $(SRC)*.cpp

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo 'could not rm';

run: default
	./bin/main -i inp.txt -o s