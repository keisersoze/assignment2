//
// Created by Filippo Maganza on 17/12/2018.
//

#include<iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"
#include "printer.h"
#include "sum.h"

int main() {

    matrix<int,4,5> A;
    for (int i=0; i!=4; ++i)
        for(int j=0; j!=5; ++j)
            A(i,j) = i*3+j;

    matrix<int> C(2,3);
    for (int i=0; i!=2; ++i)
        for(int j=0; j!=3; ++j)
            C(i,j) = i*2+2;

    matrix<int> D(5,2);
    for (int i=0; i!=5; ++i)
        for(int j=0; j!=2; ++j)
            D(i,j) = i+j;

    matrix<int,5,4> F;
    for (int i=0; i!=5; ++i)
        for(int j=0; j!=4; ++j)
            F(i,j) = i+j+5;

    matrix <int> G = D.transpose().window({0,2,0,4});
    matrix <int>  B = G + A.window({1,3,2,5}) * C.transpose() * D.transpose() * F + D.transpose().window({0,2,0,4});

    pprint(B);

    return 0;

}