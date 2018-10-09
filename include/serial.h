# ifndef SERIAL_H
# define SERIAL_H

# include <vector>

# include <queue> 
# include <chemfiles.hpp>
namespace DBROLI001 {
    auto dist = [](auto & fst, auto & snd) {
        return sqrt(
            pow((fst[0] - snd[0]), 2) +
            pow((fst[1] - snd[1]), 2) +
            pow((fst[2] - snd[2]), 2)
        );
    };
    

    typedef std::vector<int> vint;
    typedef std::pair<int,int> intpair;
    typedef std::pair<double, intpair> pairint;

    struct Comparator{
        bool operator()(const pairint& p1, const pairint& p2)
        {
            return p1.first - p2.first > 0;
        }
    };
    typedef std::priority_queue<pairint, std::vector<pairint>, Comparator> pqtype;
    
    
    class serial {
        public:
            void findDistancesBetweenPoints(
                vint & setA,
                vint & setB,
                const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
                pqtype & pq
            );
            
            void hello(std::vector<int> setA);
            serial();
            ~serial();
    };
}

#endif