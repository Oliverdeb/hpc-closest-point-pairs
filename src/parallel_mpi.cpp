
# include "parallel_mpi.h"

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
std::vector<std::pair<double, std::pair<int,int>>> parallel_mpi::findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms){
    return 0;
}