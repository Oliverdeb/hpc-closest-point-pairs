
# include "parallel_mpi.h"
# include <sstream>
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
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads){
    int num_frames = file.nsteps();
    pqtype pqs[num_frames];
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    chemfiles::Frame frame;
    double begin = omp_get_wtime();

    

    for(int i = 0; i < num_frames; ++i){
        auto pq = DBROLI001::pqtype();

        frame = file.read();     
    
        // std::cout << omp_get_thread_num() << "\t" << &frame << std::endl;
        parallel_openmp::findDistancesBetweenPoints(K, setA, setB, frame.positions(), pq);
        pqs[i] = pq;        
    }

    std::cout << "Time taken: " << omp_get_wtime() - begin << std::endl;
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