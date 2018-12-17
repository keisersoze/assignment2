//
// Created by Filippo Maganza on 13/12/2018.
//

#ifndef ASSIGNMENT2_MULTIPLICATION_PROXY_H
#define ASSIGNMENT2_MULTIPLICATION_PROXY_H

#include<iostream>

#include<vector>
#include "matrix.h"
#include "matrix_wrap.h"
#include "printer.h"


/*
 * multiply two matrices_wrap of templated type R and U
 */
template< class R, class U>
matrix_wrap<decltype(R()*U())> do_multiplication(const matrix_wrap<R>& m1,const matrix_wrap<U>& m2){
    matrix<decltype(R()*U())> result(m1.get_height(),m2.get_width());
    if (m1.get_width() != m2.get_height())
        throw "!!!";
    for (int k = 0; k < m1.get_height() ; ++k) {
        decltype(R()+U()) buff(0);
        for (int i = 0; i < m2.get_width() ; ++i) {
            for (int j = 0; j < m1.get_width(); ++j) {
                buff += m1(k,j) * m2 (j,i);
            }
            result(k,i)=buff;
            buff=0;
        }
    }
    return matrix_wrap<decltype(R()*U())>(result);
}

template <typename T>
class multiplication_proxy {
private:

    std::vector<matrix_wrap<int>> vector;

    /* find the current optimal multiplication
     * Pre: the size of the vector must be >= 2
     * Time complexity: O(n), with n equal to the size of the vector
     */
    inline unsigned find_optimal_multiplication () const{
        if (vector.size()<2)
            throw "!!!";
        unsigned current_opt = vector[0].get_width();
        unsigned opt_idx = 0;
        for (unsigned i = 0; i < vector.size() - 1 ; ++i) {
            unsigned current_size = vector[i].get_width();
            if (current_opt > vector[i].get_width()) {
               current_opt = current_size;
               opt_idx = i;
            }
        }
        return opt_idx;
    }


public:

    /*
    template<typename U, class dec, unsigned height, unsigned width>
    multiplication_proxy &operator*(const matrix_ref<U, dec, height, width > &m) {

    }*/

    template<class dec>
    multiplication_proxy &operator*(const matrix_ref<T, dec> &m) {
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(matrix_wrap<int>(m)); // emplace back
        return *this;
    }

    multiplication_proxy &operator*(const matrix_wrap<T> &m) {
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(m); // deep copy of matrix_wrap
        return *this;
    }

    //trigger

    operator matrix_wrap<T>(){
        for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());
        std::cout<< "----------------\n";

        while(vector.size() > 2) {
            unsigned opt = find_optimal_multiplication();
            auto result = do_multiplication(vector[opt], vector[opt + 1]);
            vector.erase(vector.begin() + opt);
            vector[opt]= result;

            for (auto i=vector.begin(); i<vector.end(); ++i)
                pprint_wrap(*i,(*i).get_height(),(*i).get_width());
            std::cout<< "----------------\n";

        }
        return do_multiplication(vector[0],vector[1]);
    }

    operator matrix<T>(){ //TODO is necessary?
        /*for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());

        while(vector.size() > 2) {
            unsigned opt = find_optimal_multiplication();
            auto result = do_multiplication(vector[opt], vector[opt + 1]);
            vector.erase(vector.begin() + opt);
            vector[opt]= result;
        }

        for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());



        for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());

        while (vector.size()>2) {
            std::pair<unsigned, unsigned> pair = find_optimal_multiplication();
            //result = do_multiplication(vector[pair.first], vector[pair.second]);
            auto i = vector.size();
            vector.erase(vector.begin()+pair.first);
            i =vector.size();
        }*/

        matrix<int> A(5,5);
        for (int i=0; i!=5; ++i)
            for(int j=0; j!=5; ++j)
                A(i,j) = 10+ i*10+j;

        return A;
    }

};

//OVERLOADS

//overloads for the sum
template<class T,class U, class dec, class dec2>
matrix<decltype(T()+U())> operator+ (const matrix_ref<T,dec>& m, const matrix_ref<U,dec2>& m2){
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+m2(i,j);
        }
    }
    return result;
}


template<class T, class U>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m, const matrix_wrap<T>& m2){
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+m2(i,j);
        }
    }
    return result;
}

template<class T,class U, class dec>
matrix<decltype(T()+U())> operator+ (const matrix_ref<T,dec>& m, const matrix_wrap<U>& m2){
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+m2(i,j);
        }
    }
    return result;
}

template<class T,class U, class dec>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<T>& m, const matrix_ref<U,dec>& m2){
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+m2(i,j);
        }
    }
    return result;
}

//overloads for the product

template<class T, class dec, class dec2>
multiplication_proxy<T> operator* (const matrix_ref<T,dec>& m, const matrix_ref<T,dec2>& m2){
    multiplication_proxy<T> result;
    result.operator*(m);
    result.operator*(m2);
    return result;
}

template<class T>
multiplication_proxy<T> operator* (const matrix_wrap<T>& m, const matrix_wrap<T>& m2){
    multiplication_proxy<T> result;
    result.operator*(m);
    result.operator*(m2);
    return result;
}

template<class T, class dec>
multiplication_proxy<T> operator* (const matrix_ref<T,dec>& m, const matrix_wrap<T>& m2){
    multiplication_proxy<T> result;
    result.operator*(m);
    result.operator*(m2);
    return result;
}

template<class T, class U, class dec>
multiplication_proxy<T> operator* ( const matrix_wrap<T>& m2, const matrix_ref<T,dec>& m){
    multiplication_proxy<T> result;
    result.operator*(m);
    result.operator*(m2);
    return result;
}


#endif //ASSIGNMENT2_MULTIPLICATION_PROXY_H