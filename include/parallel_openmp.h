# ifndef PARALLEL_OPENMP_H
# define PARALLEL_OPENMP_H

# include <vector>

namespace DBROLI001 {

    class parallel_openmp {
        public:
            std::vector<std::pair<double, std::pair<int,int>>> findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms);
            parallel_openmp();
            ~parallel_openmp();
    };
}

#endif