# ifndef PARALLEL_OPENMP_H
# define PARALLEL_OPENMP_H

# include <vector>

namespace DBROLI001 {

    class parallel_openmp {
        public:
            std::vector<DBROLI001::pairint> findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                chemfiles::span<chemfiles::Vector3D> & atoms,
                std::priority_queue<pairint, std::vector<pairint>, DBROLI001::Comparator> & pq
            );
            parallel_openmp();
            ~parallel_openmp();
    };
}

#endif