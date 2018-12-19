//
// Created by Filippo Maganza on 16/12/2018.
//

#ifndef ASSIGNMENT2_AWESOME_PRINT_H
#define ASSIGNMENT2_AWESOME_PRINT_H

#include<iostream>

#include "matrix_wrap.h"

template<typename T>
void pprint(const T& matrix){
    int i = 0;
    for (auto iter = matrix.begin(); iter != matrix.end(); ++iter) {
        std::cout << *iter << " ";
        if (++i % matrix.get_width() == 0) std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename T>
void pprint_wrap(matrix_wrap<T> matrix, unsigned w , unsigned h){
    for (int i=0; i!=w; ++i) {
        for(int j=0; j!=h; ++j)
            std::cout << matrix(i,j) << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';
}

#endif //ASSIGNMENT2_AWESOME_PRINT_H
