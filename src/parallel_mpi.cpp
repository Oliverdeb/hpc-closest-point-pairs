
# include "parallel_mpi.h"
# include <fstream>
# include "serial.h"
# include <mpi.h>

using namespace DBROLI001;

parallel_mpi::parallel_mpi(){
    // default constructor
}
parallel_mpi::~parallel_mpi(){
    // destrutor
}

// send X frames at a time to be processed
// send one frame ?
// Only send K back.
void solveMPI(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads){

    MPI_Init(NULL, NULL);

    // Get rank and the number of processes
    int num_procs, proc_id;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

    double begin = MPI_Wtime();
    int num_frames = file.nsteps();
    pqtype pqs[num_frames];
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    chemfiles::Frame frame;

    int timestep = 0;
    while (timestep < file.nsteps()) {
        auto pq = DBROLI001::pqtype();
        // frame = file.read();     
        if (proc_id == 0) { // hnad out work from master
            for (int i = 0; i < num_procs; ++i){
                MPI_Send(&file.read(), 1, chemfiles::Frame, 0, 1, MPI_COMM_WORLD);
            }
        }else{ // worker nodes, get to work!
            
        }
    
        serial::findDistancesBetweenPoints(K, setA, setB, frame.positions(), pq);
        pqs[i] = pq;        
    }
    MPI_Finalize();
    std::cout << "Time taken: " << MPI_Wtime() - begin << std::endl;

    for (int i =0; i < sizeof(pqs)/sizeof(pqtype); ++i){        

        auto & pq = pqs[i];
        std::vector<pairint> reversed;
        for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }
        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];            
            output << i << ',' 
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }

    
    //
}