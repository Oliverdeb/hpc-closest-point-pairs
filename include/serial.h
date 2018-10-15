# ifndef SERIAL_H
# define SERIAL_H

# include <vector>
# include <queue> 
# include <chemfiles.hpp>
# include <cmath> 
# include <iostream>

namespace DBROLI001 {
    auto dist = [](auto & fst, auto & snd) -> double {
        return std::sqrt(
            std::pow((fst[0] - snd[0]), 2) +
            std::pow((fst[1] - snd[1]), 2) +
            std::pow((fst[2] - snd[2]), 2)
        );        
    };    

    typedef std::vector<int> vint;
    typedef std::pair<int,int> intpair;
    typedef std::pair<double, intpair> pairint;

    struct Comparator{
        bool operator()(const pairint& p1, const pairint& p2)
        {
            // std::cout << (double)p1.first << "\ns";
            return p1.first < p2.first;
        }
    };
    typedef std::priority_queue<pairint, std::vector<pairint>, Comparator> pqtype;
    
    
    class serial {
        public:
            void findDistancesBetweenPoints(int K,
                const vint & setA,
                const vint & setB,
                const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
                pqtype & pq);

            void solveSerial(unsigned int K,
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                chemfiles::Trajectory & file);
            
            serial();
            ~serial();
    };
}

#endif