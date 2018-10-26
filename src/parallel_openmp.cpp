
# include "parallel_openmp.h"
// # include "serial.h"
// # include <chemfiles.hpp>
// #include "array_tools.hpp"
// #include "dcd_r.hpp"
# include <cmath> 
# include <omp.h>
# include <fstream>
using namespace DBROLI001;

parallel_openmp::parallel_openmp(){
    // default constructor
}
parallel_openmp::~parallel_openmp(){
    // destrutor
}

void DBROLI001::findDistancesBetweenPoints(int K,
    const DBROLI001::vint & setA,
    const DBROLI001::vint & setB,
    DCD_R & file,
    DBROLI001::pqtype & pq){

    const float * xs = file.getX();
    const float * ys = file.getY();
    const float * zs = file.getZ();
        
    for(const int & a_index : setA){
        // std::cout << "index"  << p1_index;
        // auto & p1 = atoms[p1_index];
        for(const int & b_index : setB){
            // auto & p2 = atoms[p2_index];
            auto pair = std::make_pair(
                std::sqrt(
                    std::pow(xs[a_index] - xs[b_index], 2) +
                    std::pow(ys[a_index] - ys[b_index], 2) +
                    std::pow(zs[a_index] - zs[b_index], 2)
                ),
                std::make_pair(a_index, b_index)
            );
            
            if (pq.size() < K){         
                pq.push(pair);
            }else{
                if (pq.top().first > pair.first){
                    pq.pop();
                    pq.push(pair);           
                }     
            }
        }
    }
}

void parallel_openmp::findDistancesBetweenPoints_directly(int K,
                std::vector<std::vector<float>> as,
                std::vector<std::vector<float>> bs,
                DBROLI001::pqtype & pq){
        
    for(const std::vector<float> & p1 : as){
        // std::cout << "index"  << p1_index;
        // auto & p1 = atoms[p1_index];
        for(const std::vector<float> & p2 : bs){
            // auto & p2 = atoms[p2_index];
            auto pair = std::make_pair(
                std::sqrt(
                    std::pow(p1[1] - p2[1], 2) +
                    std::pow(p1[2] - p2[2], 2) +
                    std::pow(p1[3] - p2[3], 2)
                ),
                std::make_pair(p1[0], p2[0])
            );
            
            if (pq.size() < K){         
                pq.push(pair);
            }else{
                if (pq.top().first > pair.first){
                    pq.pop();
                    pq.push(pair);           
                }     
            }
        }
    }
}


void parallel_openmp::solveOpenMP(unsigned int K,
            std::ofstream & output,
            const vint & setA,
            const vint & setB,
            // std::vector<chemfiles::Trajectory> & files,
            DCD_R & file,
            const unsigned int & num_threads){  
    
    int num_frames = file.getNFILE();
    pqtype pqs[num_frames];
    unsigned int i;
    omp_set_num_threads(num_threads);
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    int last_step[num_threads];
    for (int i = 0; i < num_threads; ++i)
        last_step[i] = 0;
    // chemfiles::Frame frame;
    auto pq = DBROLI001::pqtype();
    double begin = omp_get_wtime();
    int sequential_index = 0, my_index;
    std::vector<std::vector<float>> as;
    std::vector<std::vector<float>> bs;

    #pragma omp parallel for private(i, pq, my_index, as,  bs)  shared(pqs, sequential_index)  
    for(i = 0; i < num_frames; ++i){
        const int & curr_thread_num = omp_get_thread_num();
        
        pq = DBROLI001::pqtype();
        #pragma omp critical
        {
            file.read_oneFrame();
            const float * xs = file.getX();
            const float * ys = file.getY();
            const float * zs = file.getZ();  
            for (const int & index : setA)
                as.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));

            for (const int & index : setB)
                bs.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));
            
            my_index = sequential_index;
            sequential_index++;
        }
    
        // std::cout << omp_get_thread_num() << "\t" << &frame << std::endl;
        parallel_openmp::findDistancesBetweenPoints_directly(K, as, bs, pq);
        as.clear();
        bs.clear();
        pqs[my_index] = pq;        
        // std::cout << "loop end" << std::endl;
    }
    std::cout << std::endl << "Time taken: " << omp_get_wtime() - begin << std::endl;
    for (int i =0; i < sizeof(pqs)/sizeof(pqtype); ++i){

        auto & pq = pqs[i];
        std::vector<pairint> reversed;
        for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }
        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];            
            output << i << ',' << std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }

    
    
}

void parallel_openmp::openmp_for_mpi(unsigned int K,
            unsigned int start,
            unsigned int end,
            std::ofstream & output,
            const vint & setA,
            const vint & setB,
            // std::vector<chemfiles::Trajectory> & files,
            DCD_R & file,
            const unsigned int & num_threads){  
    
    int num_frames = end - start;
    pqtype pqs[num_frames];
    unsigned int i;
    omp_set_num_threads(num_threads);
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    auto pq = DBROLI001::pqtype();
    double begin = omp_get_wtime();
    int sequential_index = 0, my_index;
    std::vector<std::vector<float>> as;
    std::vector<std::vector<float>> bs;
    #pragma omp parallel for private(i, as, bs, pq, my_index)  shared(pqs, sequential_index)  
    for(i = start; i < end; ++i){
        // const int & curr_thread_num = omp_get_thread_num();
        
        pq = DBROLI001::pqtype();
        #pragma omp critical
        {
            // std::cout << "thread: "<< curr_thread_num << " i: " << i << " sequential index " << sequential_index << std::endl;
            file.read_oneFrame();
            const float * xs = file.getX();
            const float * ys = file.getY();
            const float * zs = file.getZ();  
            for (const int & index : setA)
                as.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));

            for (const int & index : setB)
                bs.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));
            
            my_index = sequential_index;
            sequential_index++;
        }
    
        parallel_openmp::findDistancesBetweenPoints_directly(K, as, bs, pq);
        as.clear();
        bs.clear();
        pqs[my_index] = pq;        
    }
    // std::cout << std::endl << "Time taken: " << omp_get_wtime() - begin << std::endl;
    for (int i =0; i < sizeof(pqs)/sizeof(pqtype); ++i){

        auto & pq = pqs[i];
        std::vector<pairint> reversed;
        for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }
        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];            
            output << start+i << ',' << std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }
}

void parallel_openmp::openmp_for_mpi(unsigned int K,
            unsigned int start,
            unsigned int end,
            std::stringstream & output,
            const vint & setA,
            const vint & setB,
            // std::vector<chemfiles::Trajectory> & files,
            DCD_R & file,
            const unsigned int & num_threads){  
    
    int num_frames = end - start;
    pqtype pqs[num_frames];
    unsigned int i;
    omp_set_num_threads(num_threads);
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    // chemfiles::Frame frame;
    auto pq = DBROLI001::pqtype();
    double begin = omp_get_wtime();
    int sequential_index = 0, my_index;
    std::vector<std::vector<float>> as;
    std::vector<std::vector<float>> bs;
    #pragma omp parallel for private(i, as, bs, pq, my_index)  shared(pqs, sequential_index)  
    for(i = start; i < end; ++i){
        // const int & curr_thread_num = omp_get_thread_num();
        
        pq = DBROLI001::pqtype();
        #pragma omp critical
        {
            // std::cout << "thread: "<< curr_thread_num << " i: " << i << " sequential index " << sequential_index << std::endl;
            file.read_oneFrame();
            const float * xs = file.getX();
            const float * ys = file.getY();
            const float * zs = file.getZ();  
            for (const int & index : setA)
                as.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));

            for (const int & index : setB)
                bs.push_back(std::vector<float>({(float)index, xs[index], ys[index], zs[index]}));
            
            my_index = sequential_index;
            sequential_index++;
        }
    
        parallel_openmp::findDistancesBetweenPoints_directly(K, as, bs, pq);
        as.clear();
        bs.clear();
        pqs[my_index] = pq;        
    }
    // std::cout << std::endl << "Time taken: " << omp_get_wtime() - begin << std::endl;
    for (int i =0; i < sizeof(pqs)/sizeof(pqtype); ++i){

        auto & pq = pqs[i];
        std::vector<pairint> reversed;
        for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }
        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];            
            output << i << ',' << std::fixed << std::setprecision(6)
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }
}