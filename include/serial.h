# ifndef SERIAL_H
# define SERIAL_H

# include <vector>

namespace DBROLI001 {

    class serial {
        public:
            std::vector<std::pair<double, std::pair<int,int>>> findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms);
            serial();
            ~serial();
    };
}

#endif