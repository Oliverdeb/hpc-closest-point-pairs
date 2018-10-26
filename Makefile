# Remove -cxx=g++-5 below if compiler gives error about unknown version g++-5
MPICC=mpic++  -cxx=g++-5
SRC=src/
BINDIR=bin/
CHEMDIR_RPI=chemfiles_compiled_for_RPi/
CHEMDIR=chemfiles/
INCLUDE=include/
FLAGS= -std=c++14 -I$(INCLUDE) -I$(CHEMDIR)$(INCLUDE) -lchemfiles -L $(CHEMDIR)lib  -Ofast #-O2 -ffast-math #-Wall -Wextra
RPI_FLAGS = -std=c++14 -I$(INCLUDE) -I$(CHEMDIR_RPI)$(INCLUDE) -lchemfiles -L $(CHEMDIR_RPI)lib  -Ofast #-O2 -ffast-math #-Wall -Wextra
OPENMP=-fopenmp

INPUT_FILE=inp.txt
# INPUT_FILE=example_input_file2.txt
OUTPUT_FILE=out.txt

default: clean
	$(MPICC) $(OPENMP) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS) 

rpi: clean
	$(MPICC) $(OPENMP) $(SRC)*.cpp -o $(BINDIR)main $(RPI_FLAGS)

mpi: clean
	$(MPICC) $(OPENMP) $(SRC)*.cpp -o $(BINDIR)main $(FLAGS)

runopenmp: default
	./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 4 -openmp

runserial: default
	./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 2 -serial

runmpi: default
	mpirun -n 2 ./bin/main -i $(INPUT_FILE) -o $(OUTPUT_FILE) -thread 2 -mpi

clean: 
	@ [ -e  $(BINDIR)main ] && rm  $(BINDIR)main || echo '';

run: default runserial
