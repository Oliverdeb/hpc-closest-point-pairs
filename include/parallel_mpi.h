# ifndef PARALLEL_MPI_H
# define PARALLEL_MPI_H

# include <vector>
# include "serial.h"

namespace DBROLI001 {

    class parallel_mpi {
        public:
            void solveMPI(unsigned int K,
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                // std::vector<chemfiles::Trajectory> & files,
                chemfiles::Trajectory & file,
                const unsigned int & num_threads);
                
            parallel_mpi();
            ~parallel_mpi();
    };
}

#endif