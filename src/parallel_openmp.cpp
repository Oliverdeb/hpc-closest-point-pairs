
# include "parallel_openmp.h"
# include "serial.h"
using namespace DBROLI001;

parallel_openmp::parallel_openmp(){
    // default constructor
}
parallel_openmp::~parallel_openmp(){
    // destrutor
}

void parallel_openmp::findDistancesBetweenPoints(
    DBROLI001::vint & setA,
    DBROLI001::vint & setB,
    chemfiles::span<chemfiles::Vector3D> & atoms,
    DBROLI001::pqtype & pq){

    
    
    for(int & p1_index : setA){
        auto p1 = atoms[p1_index];
        for(int & p2_index : setB){
            auto p2 = atoms[p2_index];
            pq.push(
                std::make_pair(
                    dist(p1, p2),
                    std::make_pair(p1_index, p2_index)
                )
            );
        }
    }

}