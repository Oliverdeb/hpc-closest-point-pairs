<!-- Please view in a markdown viewer -->
# Compiling and running (C++ implementation)

## Compile (all approaches built in one binary, selected by command line switches) & run
## Requirements
- GCC 5+
- MPICH 3 (OpenMPI might work, had some compile errors on Hex using OpenMPI linker)


### Compile (binary gets compiled and dumped into ./bin/ )
NOTE: I used MPICH as my MPI implementation (Rasperry Pi cluster was setup using MPICH). To compile locally, I had to add the flag `-cxx=g++-5` to all my compilations as I had MPICH compiled with GCC5 but my default GCC was GCC7. This caused an error when using the MPICH `mpic++` linker as the compiler versions were not matching, so I forced `mpic++` to use GCC5 with `-cxx=g++-5`. This may cause a compile error for you, as you may not have g++-5 explicity named like that or you may have a MPI implementation compiled with a different gcc version. Just **remove** the flag `-cxx=g++-5` from line 2 of the Makefile and it should compile fine if mpic++ is found on your path.
```shell
make
```
### Running serial & OpenMP
Run by providing input file, output file, number of threads and the approach to run
```shell
./bin/main -i input.txt -o output.txt -thread N  -(serial/openmp/mpi)
```

OpenMP Example `./bin/main -i inp.txt -o out.txt -thread 2 -openmp`
Serial Example: `./bin/main -i inp.txt -o out.txt -thread 2 -serial` **thread** flag is still needed even though it does nothing in the serial case, I use argv[7] to choose which algorithm to run, please don't omit the thread flag.

### Running MPI, similar to above, just with mpirun, obviously:
Example `mpirun -n 2 ./bin/main -i inp.txt -o out.txt -thread 2 -mpi`. The thread flag is still needed because my MPI approach uses OpenMP to parallelize further.
