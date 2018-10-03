
GCC=g++
SRC=src/
BINDIR=bin/
CHEMDIR=chemfiles/
INCLUDE=include/
FLAGS= -std=c++17 -ffast-math -I$(INCLUDE) -I$(CHEMDIR)$(INCLUDE) -lchemfiles -L $(CHEMDIR)lib

default: clean
	$(GCC) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) 

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo 'could not rm';

run: default
	./bin/main -i inp.txt -o s