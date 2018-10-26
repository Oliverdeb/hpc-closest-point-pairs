
# include "parallel_mpi.h"
# include "parallel_openmp.h"
// # include <chemfiles.hpp>
# include <iostream>
# include <string>
# include <vector>
# include <queue>
# include <sstream>
# include <fstream>
#include "array_tools.hpp"
#include "dcd_r.hpp"
// # include "serial.h"
# include <mpi.h>
using namespace DBROLI001;

void parseCSV(std::vector<int> & set, std::istringstream & ibuff){
    int i = 0;
    while (ibuff >> i){
        set.push_back(i);

        if (ibuff.peek() == '-'){
            ibuff.ignore();
            int upper;
            ibuff >> upper;
            for (int j = i+1; j <= upper; ++j)
                set.push_back(j);            
        }

        ibuff.ignore();
    }
}

parallel_mpi::parallel_mpi(){
    // default constructor
}
parallel_mpi::~parallel_mpi(){
    // destrutor
}

// send X frames at a time to be processed
// send one frame ?
// Only send K back.
void parallel_mpi::solveMPI(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                DCD_R & file,
                int num_threads){
    int num_frames = file.getNFILE();

    MPI_Init(NULL, NULL);
    double begin = MPI_Wtime();

    // Get rank and the number of processes
    int num_procs, proc_id;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    std::cout << "proc id " << proc_id << " K IS " << K 
    << "  FILE ADDRESS" << &file 
    << std::endl;

    DBROLI001::serial serialSolver;
    DBROLI001::parallel_openmp openmp;
    int each = num_frames / num_procs;
    int start = each * proc_id;        
    int end = start + each;
    if (num_frames - end < each/2)
        end = num_frames;

    if (proc_id == 0) { // hnad out work from master
        std::cout << "Running with " << num_procs << " procs" << std::endl;
        // for (int i = 1; i < num_procs; ++i)
            // MPI_Send(&dcdfile[0], dcdfile.size(), MPI_CHAR,    
        std::cout << "master"  << std::endl;
        // serialSolver.brute_force_for_mpi(K, start, end, output, setA, setB, file);
        openmp.openmp_for_mpi(K, start, end, output, setA, setB, file, num_threads);
        
        // get output from workers add to ofstream
        std::vector<std::string> outputs(num_procs);
        for (int i = 1; i < num_procs; ++i){
            int size;
            MPI_Recv(&size, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::cout << std::endl << "PROC ID: " << i << "  Received int size: " << size << " from proc "  << i <<std::endl;
            char resp[size];
            MPI_Recv(&resp, size, MPI_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Received chars " << std::endl;
            
            outputs.push_back(std::string(resp));
            
        }

        std::cout << "--- END Time taken: " << MPI_Wtime() - begin << std::endl;
        std::cout << "outputting to file" << std::endl;

        for (std::string & s : outputs ){
            // std::cout << ">>>>>>>>>>>>>>outputting output to file" << std::endl;
            // std:: cout << s << std::endl;
            output << s;
        }
        

    }else{
        std::cout << "worker " << proc_id << " starting" << std::endl;
        // let each worker catch up to their allocated
        // piece of the file to read
        std::cout << "catching up to my start of " << start << std::endl;     
        double catch_up = MPI_Wtime();  
        // file.skip(start);
        for (int i = 0; i < start; ++i){
            // std::cout<<"\rREADINJG   " << i;
            file.read_oneFrame();
        }
        std::cout << "--- start: " << start<< " CATCH up time: " << MPI_Wtime() - catch_up << std::endl;

        std ::cout << "caught up, end set at " << end << std::endl;
        std::stringstream out_stream;
        // openmp.openmp_for_mpi(K, start, end, out_stream, setA, setB, file, num_threads);

        serialSolver.brute_force_for_mpi(K, start, end, out_stream, setA, setB, file);
        
        auto buffer = out_stream.str();
        int buffer_size = buffer.size() * sizeof(char);

        // send size of stringstream back to master
        MPI_Send(&buffer_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

        // send buffer
        MPI_Send(buffer.c_str(), buffer_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);

    }

    MPI_Finalize();

}