
# include "serial.h"
# include <cmath>
# include <iostream>
# include <sstream>
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

void serial::findDistancesBetweenPoints(int K,
                const vint & setA,
                const vint & setB,
                const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
                pqtype & pq){

    for(const int & p1_index : setA){
        auto & p1 = atoms[p1_index];
        for(const int & p2_index : setB){
            auto & p2 = atoms[p2_index];
            auto pair = std::make_pair(
                dist(p1, p2),
                std::make_pair(p1_index, p2_index)
            );

            // change to max heap
            if (pq.size() < K){         
                pq.push(pair);
            }else{
                if (pq.top().first > pair.first){
                    pq.pop();
                    pq.push(pair);           
                }     
            }
        }
    }
}

void serial::solveSerial(unsigned int K, std::stringstream & output, const vint & setA, const vint & setB, chemfiles::Trajectory & file){
    for(unsigned int i = 0; i < file.nsteps() ; ++i){
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();
        chemfiles::Frame const & frame = file.read();
        // chemfiles::Frame const & frame2 = file.read();
        // std::cout << "\t" << &frame << std::endl;
        // std::cout << "\t\t" << &frame2 << std::endl;
        findDistancesBetweenPoints(K, setA, setB, frame.positions(), pq);

        // vector to get priority queue output in reversed order
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
}