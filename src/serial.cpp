
# include "serial.h"
# include "parallel_openmp.h"
# include <cmath>
# include <iostream>
# include <fstream>
# include <utility>
# include <iomanip>
# include <queue> 
// # include <chemfiles.hpp>
#include "array_tools.hpp"
#include "dcd_r.hpp"
# include <omp.h>

using namespace DBROLI001;

serial::serial(){
    // default constructor
}
serial::~serial(){
    // destrutor
}

// double DBROLI001::dist(const chemfiles::Vector3D & fst,  const chemfiles::Vector3D & snd) {
    
//     return sqrt(
//             (double)(
//             std::pow((double)fst[0] - (double)snd[0], 2) +
//             std::pow((double)fst[1] - (double)snd[1], 2) +
//             std::pow((double)fst[2] - (double)snd[2], 2))
//         );  
//     };

// void serial::findDistancesBetweenPoints(int K,
//                 const vint & setA,
//                 const vint & setB,
//                 const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
//                 pqtype & pq){

//     for(const int & p1_index : setA){
//         auto & p1 = atoms[p1_index];
//         for(const int & p2_index : setB){
//             auto & p2 = atoms[p2_index];
//             auto pair = std::make_pair(
//                 DBROLI001::dist(p1, p2),
//                 std::make_pair(p1_index, p2_index)
//             );

            
//             if (pq.size() < K){         
//                 pq.push(pair);
//             }else{
//                 if (pq.top().first > pair.first){
//                     pq.pop();
//                     pq.push(pair);           
//                 }     
//             }
//         }
//     }
// }

void serial::solveSerial(unsigned int K,
 std::ofstream & output,
 const vint & setA,
 const vint & setB,
 DCD_R & file){
    double start = omp_get_wtime();
    int num_frames = file.getNFILE();
    // int num_steps = 10;
    std::cout << "detetected n frames: "  << num_frames << std::endl;
    pqtype pqs[num_frames];
    for(unsigned int i = 0; i < num_frames; ++i){
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();
        // chemfiles::Frame const & frame = file.getNFILE();
        file.read_oneFrame();
        DBROLI001::findDistancesBetweenPoints(K, setA, setB, file, pq);

        pqs[i] = pq;
        //  std::vector<pairint> reversed;

        //  for (int j = 0; j < K; ++j){
        //     reversed.push_back(pq.top());
        //     pq.pop();
        // }

        // for (int j = K-1; j >= 0; --j){            
        //     const auto & result = reversed[j];
        //     output << i << ',' << std::fixed << std::setprecision(15)
        //         << result.second.first << ',' 
        //         << result.second.second <<',' 
        //         << result.first 
        //         << std::endl;
        // }
        
    }
    std::cout << "Time taken: " << omp_get_wtime() - start << std::endl;
    for (int i = 0; i < num_frames; ++i){
        auto & pq = pqs[i];
        // vector to get priority queue output in reversed order
        std::vector<pairint> reversed;

         for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }

        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];
            output << i << ',' << std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }
}

void serial::brute_force_for_mpi(unsigned int K,
    unsigned int start,
    unsigned int end,
    std::stringstream & output,
    const vint & setA,
    const vint & setB,
    DCD_R & file){

    int num_steps = end - start;
    for(unsigned int i = start; i < end; ++i){
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();
        file.read_oneFrame();

        DBROLI001::findDistancesBetweenPoints(K, setA, setB, file, pq);
         std::vector<pairint> reversed;

         for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }

        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];
            output << i << ',' << std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }
}

void serial::brute_force_for_mpi(unsigned int K,
    unsigned int start,
    unsigned int end,
    std::ofstream & output,
    const vint & setA,
    const vint & setB,
    DCD_R & file){

    int num_steps = end - start;
    for(unsigned int i = start; i < end; ++i){
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();
        file.read_oneFrame();
        DBROLI001::findDistancesBetweenPoints(K, setA, setB, file, pq);
         std::vector<pairint> reversed;

         for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }

        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];
            output << i << ','<< std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }
}
