
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
                std::priority_queue<pairint, std::vector<pairint>> & pq){

    auto dist = [](auto & fst, auto & snd) {
        return sqrt(
            pow((fst[0] - snd[0]), 2) +
            pow((fst[1] - snd[1]), 2) +
            pow((fst[2] - snd[2]), 2)
        );
    };

    double tst[] = {1,2,3};
    double tst2[] = {1,2,3};


    // std::cout << dist(tst, tst2) << std::endl;


    // for(typename std::vector<decltype(atoms.get(setA.get(0)))>::iterator fst = setA.begin(); fst != setA.end(); ++fst){
    //     auto p1 = atoms.get(*fst);
    //     for(typename std::vector<decltype(p1)>::iterator snd = setB.begin(); snd != setB.end(); ++snd){
    //         auto p2 = atoms.get(*snd);
    //         pq.insert(std::make_pair(dist(p1, p2), std::make_pair(*fst, *snd)));
    //     }
    // }


}