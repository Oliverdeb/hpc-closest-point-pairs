
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
            // std::vector<chemfiles::Trajectory> & files,
            chemfiles::Trajectory & file,
            const unsigned int & num_threads){  
    
    int num_frames = file.nsteps();
    pqtype pqs[num_frames];
    unsigned int i;
    omp_set_num_threads(num_threads);
    std::cout << "Running with " << num_threads << " threads" << std::endl;

    int last_step[num_threads];
    for (int i = 0; i < num_threads; ++i)
        last_step[i] = 0;

    #pragma omp parallel for private(i)  //shared(pqs)  
    for(i = 0; i < num_frames; ++i){
        const int & curr_thread_num = omp_get_thread_num();
        // std::cout << "loopy" << std::endl;
        // std::cout << "\rdoing " << i ;
        auto pq = DBROLI001::pqtype();

        // std::cout << "thread: "<< curr_thread_num << "\ttrying to read " << i << std::endl;
        // std::cout << "thread: "<< curr_thread_num << "\tlast read was" <<last_step[curr_thread_num]<< " need to read " << i << std::endl;
        chemfiles::Frame frame;

        // while (i > last_step[curr_thread_num] && i != last_step[curr_thread_num]) {
        //     files[curr_thread_num].read(); 
        //     last_step[curr_thread_num]++;
        // }
        // frame = files[curr_thread_num].read(); 
        // last_step[curr_thread_num]++;
        // std::cout << "thread: "<< curr_thread_num << "after" << std::endl;

        #pragma omp critical     
        frame = file.read();     
           
    
        // std::cout << omp_get_thread_num() << "\t" << &frame << std::endl;
        parallel_openmp::findDistancesBetweenPoints(K, setA, setB, frame.positions(), pq);
        pqs[i] = pq;        
        // std::cout << "loop end" << std::endl;
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