//
// Created by Filippo Maganza on 16/12/2018.
//

#ifndef ASSIGNMENT2_AWESOME_PRINT_H
#define ASSIGNMENT2_AWESOME_PRINT_H

#include<iostream>

#include "matrix_wrap.h"

template<typename T>
void pprint(const T& matrix){
    for (int i = 0; i < matrix.get_height(); ++i) {
        for (int j = 0; j < matrix.get_width(); ++j) {
            std::cout << matrix(i,j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename T>
void pprint_wrap(const matrix_wrap<T>& matrix, unsigned w , unsigned h){
    for (int i=0; i!=w; ++i) {
        for(int j=0; j!=h; ++j)
            std::cout << matrix(i,j) << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';
}

#endif //ASSIGNMENT2_AWESOME_PRINT_H
