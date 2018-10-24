
GCC=mpic++ -cxx=g++-5
MPICC=mpic++ -cxx=g++-5
SRC=src/
BINDIR=bin/
CHEMDIR=chemfiles/
INCLUDE=include/
FLAGS= -std=c++14 -I$(INCLUDE) -I$(CHEMDIR)$(INCLUDE) -lchemfiles -L $(CHEMDIR)lib  -Ofast #-O2 -ffast-math #-Wall -Wextra
OPENMP=-fopenmp

# INPUT_FILE=inp.txt
INPUT_FILE=example_input_file2.txt
OUTPUT_FILE=out.txt

default: clean
	$(GCC) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) 

openmp: clean
	$(GCC) $(OPENMP) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) 

mpi: clean
	$(MPICC) $(OPENMP) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS)

runopenmp: openmp
	time ./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 4 -openmp

runserial: openmp
	time ./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 2 -serial

runmpi: mpi
	mpirun -n 2 ./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 2 -mpi

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo 'could not rm';

run: default runserial