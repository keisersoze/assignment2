//
// Created by Filippo Maganza on 13/12/2018.
//

#include<iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"
#include "printer.h"

int main() {

    matrix<int,2,3> A;
    for (int i=0; i!=3; ++i)
        for(int j=0; j!=3; ++j)
            A(i,j) = i*3+j;

    matrix<int> C(3,2);
    for (int i=0; i!=3; ++i)
        for(int j=0; j!=3; ++j)
            C(i,j) = i*2+2;

    matrix<int> D(2,5);
    for (int i=0; i!=1; ++i)
        for(int j=0; j!=3; ++j)
            D(i,j) = i+j;

    //matrix_wrap<int> F(D);

    matrix_wrap <int> B = A * C * D;

    //pprint(A);
    //pprint(C);
    //pprint(D);
    pprint_wrap(B,B.get_height(),B.get_width());

    return 0;

}
