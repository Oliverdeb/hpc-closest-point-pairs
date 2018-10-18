
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
# include <omp.h>

void parse_csv(std::vector<int> & set, std::istringstream & ibuff){
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

void write_output_to_file(std::string & outfile, std::stringstream & output){
    std::ofstream out(outfile);
    out.precision(15);
    out << output.rdbuf();
    out.close();
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
    if (argc > 4 && strcmp(argv[5], "-thread") == 0){
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
    
    parse_csv(setA, ibuff);
    // output_vector(setA);

    std::getline(infile, line);
    while (line == "")
        std::getline(infile, line);

    ibuff = std::istringstream(line);
    parse_csv(setB, ibuff);
    // output_vector(setB);    
    infile.close();
    std::cout << "Set A: " << *setA.begin() << '-' << *(setA.end() - 1) << std::endl;
    std::cout << "Set B: " << *setB.begin() << '-' << *(setB.end() - 1) << std::endl << std::endl;
    
    
    DBROLI001::serial serialSolver;
    DBROLI001::parallel_openmp openmpSolver;
    
    std::stringstream output;
    output.precision(15);
    chemfiles::Trajectory file(dcdfile);

    if (strcmp(argv[7], "-serial") == 0){
        // chemfiles::Trajectory file(dcdfile);
        std::cout << "\n\nRunning serial version" << std::endl;
        serialSolver.solveSerial(K, output, setA, setB, file);

    }else if (strcmp(argv[7], "-openmp") == 0){
        // std::vector<chemfiles::Trajectory> files;
        // for (int i = 0; i < num_threads; ++i)
        //     files.push_back(chemfiles::Trajectory(dcdfile));

        std::cout << "\n\nRunning OpenMP version" << std::endl;
        openmpSolver.solveOpenMP(K, output, setA, setB, file, num_threads);

    } else {
        // mpiSolver.solveMPI
        
    }
    
    write_output_to_file(o_file, output);
    return 0;
}
