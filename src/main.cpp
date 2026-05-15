#include <iostream>
#include "sequence.hpp"
#include "ArraySequence.hpp"
#include "LinkedList.hpp"
bool is_big(int x){ 
    return x > 60; 
}


int main(){

    Sequence<int> *A;
    int C[6] = {1,2,3,4,5, 120};

    ArraySequence<int> B(C, 6);
    SequenceStats<int> sss{};
    sss = B.get_stats();
    
    std::cout << sss.max << " " << sss.min << " " << sss.avg;




    return 0;
}










