# ifndef SERIAL_H
# define SERIAL_H

# include <vector>

# include <queue> 
# include <chemfiles.hpp>
namespace DBROLI001 {

    typedef std::vector<int> vint;
    typedef std::pair<int,int> intpair;
    typedef std::pair<double, intpair> pairint;

    struct Comparator{
        bool operator()(const pairint& p1, const pairint& p2)
        {
            return p1.first - p2.first < 0;
        }
    };

    
    class serial {
        public:
            void findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                chemfiles::span<chemfiles::Vector3D> & atoms,
                std::priority_queue<pairint, std::vector<pairint>, Comparator> & pq);
            void hello(std::vector<int> setA);
            serial();
            ~serial();
    };
}

#endif