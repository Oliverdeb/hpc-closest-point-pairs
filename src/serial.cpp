
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

void serial::hello(std::vector<int> setA){    
    std::cout << "hello from test=" << std::endl;
}

// double dist(auto & fst, auto & snd){
//     return 0;
// }

void serial::findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                chemfiles::span<chemfiles::Vector3D> & atoms,
                std::priority_queue<pairint, std::vector<pairint>, DBROLI001::Comparator> & pq){

    auto dist = [](auto & fst, auto & snd) {
        return sqrt(
            pow((fst[0] - snd[0]), 2) +
            pow((fst[1] - snd[1]), 2) +
            pow((fst[2] - snd[2]), 2)
        );
    };



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