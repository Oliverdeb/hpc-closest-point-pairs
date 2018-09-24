
# include "serial.h"
# include  <iostream>
using namespace DBROLI001;

serial::serial(){
    // default constructor
}
serial::~serial(){
    // destrutor
}

std::vector<std::pair<double, std::pair<int,int>>> serial::findDistancesBetweenPoints(auto & setA, auto & setB, auto & atoms){
    // std::cout << 
    auto fst = setA.size() > setB.size() ? setA : setB; 
    
}