# ifndef PARALLEL_OPENMP_H
# define PARALLEL_OPENMP_H


# include <vector>
# include <queue> 
// # include <chemfiles.hpp>
#include "array_tools.hpp"
#include "dcd_r.hpp"
# include "serial.h"
namespace DBROLI001 {

    extern void findDistancesBetweenPoints(int K,
                const DBROLI001::vint & setA,
                const DBROLI001::vint & setB,
                DCD_R & file,
                DBROLI001::pqtype & pq);

    class parallel_openmp {
        public:
        void nestedfindDistancesBetweenPoints(int K,
                const DBROLI001::vint & setA,
                const DBROLI001::vint & setB,
                DCD_R & file,
                DBROLI001::pqtype & pq);
            void findDistancesBetweenPoints(int K,
                const DBROLI001::vint & setA,
                const DBROLI001::vint & setB,
                DCD_R & file,
                DBROLI001::pqtype & pq);

            void findDistancesBetweenPoints_directly(int K,
                std::vector<std::vector<float>> as,
                std::vector<std::vector<float>> bs,
                DBROLI001::pqtype & pq);

            void solveOpenMP(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                DCD_R & file,
                const unsigned int & num_threads);

            void openmp_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                DCD_R & file,
                const unsigned int & num_threads);

            void openmp_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                DCD_R & file,
                const unsigned int & num_threads);
            parallel_openmp();
            ~parallel_openmp();
    };
}

#endif