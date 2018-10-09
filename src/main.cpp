
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
    out << output.rdbuf();
    out.close();
}


int main(int argc, char *argv[])
{
    std::string i_file = "";
    std::string o_file = "";
    int K = 0;
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
    infile >> dcdfile >> K >> std::ws;
    std::cout << "dcd is "<< dcdfile << std::endl;
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


    chemfiles::Trajectory file("example_pn3_10RU_751frames.dcd");
    
    // std::cout << file.nsteps() << std::endl;
    // auto positions = frame.positions();
    // std::cout << "first frame index 0" << positions[0][0] << ", " << positions[0][1] << ", " << positions[0][2] << std::endl;
    // std::cout << "second frame index 0" << next[0][0] << ", " << next[0][1] << ", " << next[0][2] << std::endl;
    // for (auto & pos : positions)
        // std::cout << pos[0] << ", " <<  pos[1] << ", " << pos[2] << std::endl;

    DBROLI001::serial serialSolver;
    DBROLI001::parallel_openmp openmpSolver;
    
    DBROLI001::pqtype pq;
    // std::vector<std::string> output;
    std::stringstream output;

    for(int i = 0; i < file.nsteps(); ++i){
        std::cout << "doing " << i << std::endl;
        pq = DBROLI001::pqtype();
        chemfiles::Frame const & frame = file.read();
        serialSolver.findDistancesBetweenPoints(setA, setB, frame.positions(), pq);

        for (int j = 0; j < K; ++j){
            const auto & result = pq.top();
            pq.pop();
            output << i << ',' << result.second.first << ',' << result.second.second <<',' << result.first << std::endl;
        }
    }
    write_output_to_file(o_file, output);

    std::cout << pq.top().first << std::endl;
    pq.pop();
    std::cout << pq.top().first << std::endl;
    // serial_pq.pop();serial_pq.pop();serial_pq.pop();serial_pq.pop();serial_pq.pop();
    // std::cout << serial_pq.top().first << std::endl;

    // DBROLI001::pqtype openmp_pq;
    // openmpSolver.findDistancesBetweenPoints(setA, setB, positions, serial_pq);



    return 0;
}
