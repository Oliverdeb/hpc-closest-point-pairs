# ifndef SERIAL_H
# define SERIAL_H

# include <vector>

# include <queue> 
# include <chemfiles.hpp>
namespace DBROLI001 {
    typedef std::vector<int> vint;
    typedef std::pair<double, std::pair<int,int>> pairint;
    class serial {
        public:
            void findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                chemfiles::span<chemfiles::Vector3D> & atoms,
                std::priority_queue<pairint, std::vector<pairint>> & pq);
            void hello(std::vector<int> setA);
            serial();
            ~serial();
    };
}

#endif