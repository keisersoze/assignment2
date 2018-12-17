//
// Created by Filippo Maganza on 13/12/2018.
//
#include<iostream>

#include "matrix.h"
#include "multiplication_proxy.h"
#include "printer.h"

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
    pprint(F);

    std::cout << std::endl;
    std::cout << std::endl;

    /*
    matrix_wrap<int> B=A.transpose();
    auto F = A + B;
    pprint(F);*/

}