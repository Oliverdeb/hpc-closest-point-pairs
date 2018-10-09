
# include "serial.h"
# include "math.h"
# include <iostream>
# include <utility>

# include <queue> 
# include <chemfiles.hpp>
using namespace DBROLI001;

serial::serial(){
    // default constructor
}
serial::~serial(){
    // destrutor
}

void serial::findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
                pqtype & pq){
    
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