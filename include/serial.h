# ifndef SERIAL_H
# define SERIAL_H

# include <vector>

namespace DBROLI001 {

    class serial {
        public:
            void findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms, auto & pq);
            serial();
            ~serial();
    };
}

#endif