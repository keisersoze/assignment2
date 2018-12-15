//
// Created by Filippo Maganza on 13/12/2018.
//
#include<iostream>

#include "matrix.h"

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

    matrix<int> A(5,5);
    for (int i=0; i!=5; ++i)
        for(int j=0; j!=5; ++j)
            A(i,j) = 10+ i*10+j;

    matrix<float> C(5,5);
    for (int i=0; i!=5; ++i)
        for(int j=0; j!=5; ++j)
            C(i,j) = 0.5+10+ i*10+j;

    pprint(A);
    pprint(C);

    auto D = A + C;
    auto E = A + C.transpose();
    auto F = C.transpose() + A;

    pprint(D);
    pprint(E);

    std::cout << std::endl;
    std::cout << std::endl;

    /*
    matrix_wrap<int> B=A.transpose();
    auto F = A + B;
    pprint(F);*/

}