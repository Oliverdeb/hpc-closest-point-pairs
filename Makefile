
GCC=g++
SRC=src/
BINDIR=bin/
CHEMDIR=chemfiles/
INCLUDE=include/
FLAGS=-o $(BINDIR)main -std=c++14 -I$(INCLUDE) -I$(CHEMDIR)$(INCLUDE) -lchemfiles -L $(CHEMDIR)lib

default: clean
	$(GCC) $(SRC)*.cpp $(FLAGS) 

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo 'could not rm';

run: default
	./bin/main -i inp.txt -o s