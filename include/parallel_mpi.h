# ifndef PARALLEL_MPI_H
# define PARALLEL_MPI_H

# include <vector>
# include "serial.h"

namespace DBROLI001 {

    class parallel_mpi {
        public:
            void solveMPI(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                int num_threads);
                
            parallel_mpi();
            ~parallel_mpi();
    };
}

#endif

// void parallel_openmp::findDistancesBetweenPoints(int K,
//     const DBROLI001::vint & setA,
//     const DBROLI001::vint & setB,
//     const chemfiles::span<chemfiles::Vector3D> & atoms,
//     DBROLI001::pqtype & pq){
//     int  a = setA[0]; int  b = setB[0];
//     chemfiles::Vector3D  p1 = atoms[a]; chemfiles::Vector3D  p2 = atoms[b];
//     #pragma omp parallel for num_threads(2)  shared (atoms, pq, setA, setB) private(a, p1) 
//     for(int  a =0 ; a < setA.size(); ++a){
//         std:: cout << " LEVEL 1 " << omp_get_thread_num() << " TOTAL INNER " << omp_get_num_threads() << " a is " <<  a<< std::endl;
//         chemfiles::Vector3D  p1 = atoms[setA[a]];
//         for(int  b =0 ; b < setB.size(); ++b){
//             // chemfiles::Vector3D  p2 = ;
//             auto pair = std::make_pair(
//                 DBROLI001::dist(p1, atoms[setB[b]]),
//                 std::make_pair(setA[a], setB[b])
//             );
//             #pragma omp critical
//             {
//                 if (pq.size() < K){         
//                     pq.push(pair);
//                 }else{
//                     if (pq.top().first > pair.first){
//                         pq.pop();
//                         pq.push(pair);           
//                     }     
//                 }
//             }
//         }
//     }

// }