# ifndef PARALLEL_OPENMP_H
# define PARALLEL_OPENMP_H


# include <vector>
# include <queue> 
# include <chemfiles.hpp>
# include "serial.h"
namespace DBROLI001 {

    class parallel_openmp {
        public:
        void nestedfindDistancesBetweenPoints(int K,
                const DBROLI001::vint & setA,
                const DBROLI001::vint & setB,
                const chemfiles::span<chemfiles::Vector3D> & atoms,
                DBROLI001::pqtype & pq);
            void findDistancesBetweenPoints(int K,
                const DBROLI001::vint & setA,
                const DBROLI001::vint & setB,
                const chemfiles::span<chemfiles::Vector3D> & atoms,
                DBROLI001::pqtype & pq);

            void solveOpenMP(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads);

            void openmp_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads);

            void openmp_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads);
            parallel_openmp();
            ~parallel_openmp();
    };
}

#endif