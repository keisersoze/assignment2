//
// Created by Filippo Maganza on 13/12/2018.
//

#include<iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"
#include "printer.h"

int main() {
    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
    std::cout << "|         PRODUCT DEMO      |" << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;

    matrix<int,2,3> A;
    for (int i=0; i!=2; ++i)
        for(int j=0; j!=3; ++j)
            A(i,j) = i*3+j;

    matrix<int> C(2,3);
    for (int i=0; i!=2; ++i)
        for(int j=0; j!=3; ++j)
            C(i,j) = i*2+2;

    matrix<int> D(5,2);
    for (int i=0; i!=5; ++i)
        for(int j=0; j!=2; ++j)
            D(i,j) = i+j;

    matrix <int>  B = A * C.transpose() * D.transpose();

    pprint(B);

    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
    std::cout << "|  SIZE AT COMPILE TIME     |" << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;

    matrix<int,3,2> Z;
    for (int i=0; i!=3; ++i)
        for(int j=0; j!=2; ++j)
            Z(i,j) = i*3+j;

        /*
    matrix<int> G = A*Z;
    pprint (G);*/

    matrix<int,2,2> Yeah = A*Z*A*Z;
    pprint(Yeah);

    return 0;

}
