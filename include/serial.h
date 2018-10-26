# ifndef SERIAL_H
# define SERIAL_H

# include <vector>
# include <queue> 
// # include <chemfiles.hpp>
# include <cmath> 
# include <iostream>
# include <iomanip>
#include "array_tools.hpp"
#include "dcd_r.hpp"
namespace DBROLI001 {

    // extern double dist(const chemfiles::Vector3D & fst,  const chemfiles::Vector3D & snd);
    auto dist = [](auto * xs, auto * ys, auto * zs, auto & a_index, auto & b_index) -> double {
        return std::sqrt(
            std::pow(xs[a_index] - xs[b_index], 2) +
            std::pow(ys[a_index] - ys[b_index], 2) +
            std::pow(zs[a_index] - zs[b_index], 2)
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
            // void findDistancesBetweenPoints(int K,
            //     const vint & setA,
            //     const vint & setB,
            //     // const std::vector<chemfiles::Vector3D, std::allocator<chemfiles::Vector3D>> & atoms,
            //     DCD_R & file
            //     pqtype & pq);

            void solveSerial(unsigned int K,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                DCD_R & file);
            
            void brute_force_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::stringstream & output,
                const vint & setA,
                const vint & setB,
                DCD_R & file);

            void brute_force_for_mpi(unsigned int K,
                unsigned int start,
                unsigned int end,
                std::ofstream & output,
                const vint & setA,
                const vint & setB,
                DCD_R & file);

            serial();
            ~serial();
    };
}

#endif