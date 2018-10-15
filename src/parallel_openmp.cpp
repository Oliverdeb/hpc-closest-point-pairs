
# include "parallel_openmp.h"
# include "serial.h"
# include <omp.h>
# include <sstream>
using namespace DBROLI001;

parallel_openmp::parallel_openmp(){
    // default constructor
}
parallel_openmp::~parallel_openmp(){
    // destrutor
}

void parallel_openmp::findDistancesBetweenPoints(int K,
    const DBROLI001::vint & setA,
    const DBROLI001::vint & setB,
    const chemfiles::span<chemfiles::Vector3D> & atoms,
    DBROLI001::pqtype & pq){

    
    double max = 0.0;
    for(const int & p1_index : setA){
        auto p1 = atoms[p1_index];
        for(const int & p2_index : setB){
            auto p2 = atoms[p2_index];
            auto pair = std::make_pair(
                dist(p1, p2),
                std::make_pair(p1_index, p2_index)
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
            std::stringstream & output,
            const vint & setA,
            const vint & setB,
            chemfiles::Trajectory & file){  
    int numthreads = 2;
        // std::cout << "\rdoing " ;
    
    pqtype pqs[1];
    unsigned int i;
    #pragma omp parallel for private(i)  shared(pqs, setA, setB, file)  
    for(i = 0; i <1; ++i){
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();

        chemfiles::Frame frame;
        #pragma omp critical                 
        frame = file.read();        
    
        // std::cout << omp_get_thread_num() << "\t" << &frame << std::endl;
        parallel_openmp::findDistancesBetweenPoints(K, setA, setB, frame.positions(), pq);
        pqs[i] = pq;        
    }
    for (int i =0; i < sizeof(pqs)/sizeof(pqtype); ++i){

        auto & pq = pqs[i];
        std::vector<pairint> reversed;
        for (int j = 0; j < K; ++j){
            reversed.push_back(pq.top());
            pq.pop();
        }
        for (int j = K-1; j >= 0; --j){            
            const auto & result = reversed[j];            
            output << i << ',' 
                << result.second.first << ',' 
                << result.second.second <<',' 
                << result.first 
                << std::endl;
        }
    }

    
    
}