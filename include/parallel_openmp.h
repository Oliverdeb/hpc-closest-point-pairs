# ifndef PARALLEL_OPENMP_H
# define PARALLEL_OPENMP_H


# include <vector>
# include <queue> 
# include <chemfiles.hpp>
# include "serial.h"
namespace DBROLI001 {

    class parallel_openmp {
        public:
            void findDistancesBetweenPoints(
                DBROLI001::vint & setA,
                DBROLI001::vint & setB,
                chemfiles::span<chemfiles::Vector3D> & atoms,
                DBROLI001::pqtype & pq
            );
            parallel_openmp();
            ~parallel_openmp();
    };
}

#endif