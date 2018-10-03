
# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <queue> 
# include <chemfiles.hpp>
# include "string.h"
# include "serial.h"
# include "parallel_openmp.h"
# include "parallel_mpi.h"

void parse_csv(auto & set, auto & ibuff){
    int i = 0;
    while (ibuff >> i){
        set.push_back(i);

        if (ibuff.peek() == '-'){
            ibuff.ignore();
            int upper;
            ibuff >> upper;
            for (int j = i+1; j <= upper; ++j)
                set.push_back(j);            
        }

        ibuff.ignore();
    }
}

void output_vector(auto & vec){
    std::cout << "\n\nVector output:" << std::endl;

    for (auto & elem : vec)
        std::cout << elem << std::endl;
}


int main(int argc, char *argv[])
{
    std::string i_file = "";
    std::string o_file = "";
    int k = 0;
    std::vector<int> setA, setB;

    if (argc < 5){
        std::cout << "Please specify input and output files:\nUsage: ./main -i in_file -o out_file" << std::endl;
        exit(1);
    }

    // std::cout < "wtf " << (strcmp(argv[1], "-i")) << std::endl;
    if (strcmp(argv[1], "-i") == 0){

        i_file = argv[2];
        o_file = argv[4];
    }else{
        i_file = argv[4];
        o_file = argv[2];
    }

    std::ifstream infile(i_file);

    if (!infile){
        std::cerr << "cant open file " << i_file <<  std::endl;
        exit(1);
    }

    std::string dcdfile;
    infile >> dcdfile >> k >> std::ws;
    std::cout << "dcd is "<< dcdfile << std::endl;
    std::cout << "k is "<< k << std::endl;

    std::string line;
    std::getline(infile, line);
    std::istringstream ibuff(line);
    
    parse_csv(setA, ibuff);
    // output_vector(setA);

    std::getline(infile, line);
    while (line == "")
        std::getline(infile, line);

    ibuff = std::istringstream(line);

    parse_csv(setB, ibuff);
    // output_vector(setB);    
    infile.close();

    // READ IN DCD ?

    chemfiles::Trajectory file("example_pn3_10RU_751frames.dcd");
    chemfiles::Frame frame = file.read();
    std::cout << file.nsteps() << std::endl;
    auto positions = frame.positions();
    std::cout << "hello this is testy" << std::endl;
    // for (auto & pos : positions)
        // std::cout << pos[0] << ", " <<  pos[1] << ", " << pos[2] << std::endl;


    DBROLI001::serial serialSolver;
    
    std::priority_queue<
        std::pair<double, std::pair<int,int>>,
        std::vector<std::pair<double, std::pair<int, int>>>,
        decltype(serial::comparator)
    > pq(serial::comparator);
    serialSolver.hello(setA);
    serialSolver.findDistancesBetweenPoints(setA, setB, positions, pq);

    // std::cout << pq.top() << std::endl;
    // pq.push(10.0);
    // pq.push(1.0);
    // pq.push(2.0);
    // pq.push(3.0);

    // std::cout << pq.top() << std::endl;

    return 0;
}
