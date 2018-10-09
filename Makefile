
GCC=g++
SRC=src/
BINDIR=bin/
CHEMDIR=chemfiles/
INCLUDE=include/
FLAGS= -std=c++17 -ffast-math -I$(INCLUDE) -I$(CHEMDIR)$(INCLUDE) -lchemfiles -L $(CHEMDIR)lib
OPENMP=-fopenmp

INPUT_FILE=inp.txt
OUTPUT_FILE=out.txt

default: clean
	$(GCC) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) 

openmp: clean
	$(GCC) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) $(OPENMP)

execbinary:
	./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE)

runopenmp: openmp execbinary

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo 'could not rm';

run: default execbinary