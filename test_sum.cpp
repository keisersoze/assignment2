//
// Created by Filippo Maganza on 13/12/2018.
//
#include<iostream>

#include "matrix.h"
#include "multiplication_proxy.h"
#include "printer.h"
#include "sum.h"

int main() {

    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
    std::cout << "|         SUM DEMO          |" << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;

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
    matrix<float> E = A + C.transpose();
    auto F = C.transpose() + A;

    pprint(D);
    pprint(E);
    pprint(F);

    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
    std::cout << "|  SIZE AT COMPILE TIME     |" << std::endl;
    std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;

    matrix<int,5,5> Z;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            Z(i,j) = 2;
        }
    }

    matrix<int,5,5> Q = Z.transpose()+Z.transpose();
    pprint (Q);
    std::cout<<Q.is_ct()<<std::endl;
    std::cout<< Q.get_ct_height() << Q.get_ct_width();

}