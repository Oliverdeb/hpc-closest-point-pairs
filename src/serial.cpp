
# include "serial.h"
# include  <iostream>
using namespace DBROLI001;

serial::serial(){
    // default constructor
}
serial::~serial(){
    // destrutor
}

void serial::findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms, auto & pq){
    // std::cout << 
    auto fst = setA.size() > setB.size() ? setA : setB; 

}