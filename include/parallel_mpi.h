# ifndef PARALLEL_MPI_H
# define PARALLEL_MPI_H

# include <vector>

namespace DBROLI001 {

    class parallel_mpi {
        public:
            std::vector<std::pair<double, std::pair<int,int>>> findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms);
            parallel_mpi();
            ~parallel_mpi();
    };
}

#endif