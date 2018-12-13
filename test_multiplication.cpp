//
// Created by Filippo Maganza on 13/12/2018.
//

#include<iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"

template<typename T>
void pprint(T matrix){
    int i = 0;
    for (auto iter = matrix.begin(); iter != matrix.end(); ++iter) {
        std::cout << *iter << " ";
        if (++i % matrix.get_height() == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {

    matrix<int> A(3,3);
    for (int i=0; i!=3; ++i)
        for(int j=0; j!=3; ++j)
            A(i,j) = 10+ i*10+j;

    matrix<int> C(3,3);
    for (int i=0; i!=3; ++i)
        for(int j=0; j!=3; ++j)
            C(i,j) = 10+ i*10+j;


    matrix <int> B = A * C;
    pprint(A);
    pprint(C);
    pprint(B);
    return 0;

}
