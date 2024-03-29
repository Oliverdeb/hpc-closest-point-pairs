
# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <queue> 
// # include <chemfiles.hpp>
# include "string.h"
// # include "serial.h"
#include "array_tools.hpp"
#include "dcd_r.hpp"
# include "parallel_openmp.h"
# include "parallel_mpi.h"
# include <omp.h>
namespace DBROLI001 {
    void parse_csv(std::vector<int> & set, std::istringstream & ibuff);
}
void DBROLI001::parse_csv(std::vector<int> & set, std::istringstream & ibuff){
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

void output_vector(std::vector<int> & vec){
    std::cout << "\n\nVector output:" << std::endl;

    for (auto & elem : vec)
        std::cout << elem << std::endl;
}



int main(int argc, char *argv[])
{
    std::string i_file = "";
    std::string o_file = "";
    unsigned int K = 0;
    std::vector<int> setA, setB;

    if (argc < 5){
        std::cout << "Please specify input and output files:\nUsage: ./main -i in_file -o out_file -thread num_threads" << std::endl;
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

    // for (int i =0; i < argc; ++i)
    //     std::cout << i << argv[i] <<  strcmp(argv[i], "-thread") << std::endl;
    int num_threads = 2;
    if (argc > 5 && strcmp(argv[5], "-thread") == 0){
        num_threads = std::stoi(argv[6]);
        std::cout << "Number threads: " << num_threads << std::endl;
        #if defined(_OPENMP)
        omp_set_num_threads(num_threads);
        #endif
    }

    
    std::ifstream infile(i_file);

    if (!infile){
        std::cerr << "cant open file " << i_file <<  std::endl;
        exit(1);
    }

    std::string dcdfile;
    infile >> dcdfile >> K >> std::ws;
    std::cout << "\nDCD is "<< dcdfile << std::endl;
    std::cout << "K is "<< K << std::endl;

    std::string line;
    std::getline(infile, line);
    std::istringstream ibuff(line);
    
    DBROLI001::parse_csv(setA, ibuff);
    // output_vector(setA);

    std::getline(infile, line);
    while (line == "")
        std::getline(infile, line);

    std::istringstream ibuff2(line);
    // ibuff = std::istringstream(line);
    DBROLI001::parse_csv(setB, ibuff2);
    // output_vector(setB);    
    infile.close();
    std::cout << "Set A: " << *setA.begin() << '-' << *(setA.end() - 1) << std::endl;
    std::cout << "Set B: " << *setB.begin() << '-' << *(setB.end() - 1) << std::endl << std::endl;
    
    
    // output.precision(15);
    // chemfiles::Trajectory file(dcdfile);
    char arr[dcdfile.size()];
    for (int i = 0; i < dcdfile.size(); ++i)
        arr[i] = dcdfile[i];
    DCD_R file(dcdfile.c_str());
    file.read_header();

    if (strcmp(argv[7], "-serial") == 0){
        std::ofstream output("Serial_" + o_file);

        DBROLI001::serial serialSolver;
        // chemfiles::Trajectory file(dcdfile);
        std::cout << "\n\nRunning serial version" << std::endl;
        serialSolver.solveSerial(K, output, setA, setB, file);

    }else if (strcmp(argv[7], "-openmp") == 0){
        std::ofstream output("openmp_" + o_file);

        // std::vector<chemfiles::Trajectory> files;
        // for (int i = 0; i < num_threads; ++i)
        //     files.push_back(chemfiles::Trajectory(dcdfile));

        DBROLI001::parallel_openmp openmpSolver;
        std::cout << "\n\nRunning OpenMP version" << std::endl;
        openmpSolver.solveOpenMP(K, output, setA, setB, file, num_threads);

    } else {
        DBROLI001::parallel_mpi mpiSolver;
        std::ofstream output("MPI_" + o_file);

        std::cout << "\n\nRunning MPI version" << std::endl;
        mpiSolver.solveMPI(K, output, setA, setB, file, num_threads);
        
    }
    
    return 0;
}
