//
// Created by Filippo Maganza on 13/12/2018.
//

#ifndef ASSIGNMENT2_MULTIPLICATION_PROXY_H
#define ASSIGNMENT2_MULTIPLICATION_PROXY_H

#include<vector>
#include "matrix.h"
#include "matrix_wrap.h"

template <class T>
class multiplication_proxy;

//FUNCTIONS

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

template<class T, class U, class dec, class dec2>
multiplication_proxy<decltype(T()+U())> operator* (const matrix_ref<T,dec>& m, const matrix_ref<U,dec2>& m2){
    multiplication_proxy<decltype(T()+U())> result;
    result.operator*(m);
    result.operator*(m2);
    int i = result.get_vector().size();
    return result;
}

template<class T, class U>
multiplication_proxy<decltype(T()+U())> operator* (const matrix_wrap<T>& m, const matrix_wrap<U>& m2){
    multiplication_proxy<decltype(T()+U())> result;
    result.operator*(m);
    result.operator*(m2);
    int i = result.get_vector().size();
    return result;
}

template<class T, class U, class dec>
multiplication_proxy<decltype(T()+U())> operator* (const matrix_ref<T,dec>& m, const matrix_wrap<U>& m2){
    multiplication_proxy<decltype(T()+U())> result;
    result.operator*(m);
    result.operator*(m2);
    int i = result.get_vector().size();
    return result;
}

template<class T, class U, class dec>
multiplication_proxy<decltype(T()+U())> operator* ( const matrix_wrap<T>& m2, const matrix_ref<U,dec>& m){
    multiplication_proxy<decltype(T()+U())> result;
    result.operator*(m);
    result.operator*(m2);
    int i = result.get_vector().size();
    return result;
}

//

template< class R, class U>
matrix<decltype(R()+U())> do_multiplication(const matrix_wrap<R>& m1,const matrix_wrap<U>& m2){
    matrix<decltype(R()+U())> result(m1.get_height(),m2.get_width());
    if (m1.get_width() != m2.get_height())
        throw "!!!";
    for (int k = 0; k < m1.get_height() ; ++k) {
        decltype(R()+U()) buff(0);
        for (int i = 0; i < m2.get_width() ; ++i) {
            for (int j = 0; j < m1.get_width(); ++j) {
                buff += m1(i,j) * m2 (j,i);
            }
            result(k,i)=buff;
            buff=0;
        }
    }
    return result;
}

template <class T>
class multiplication_proxy{
private:

    std::vector<matrix_wrap<T>> vector;

public:
    template <typename U, class dec>
    multiplication_proxy& operator*(const matrix_ref<U,dec>& m){
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(matrix_wrap<T>(m));
        return *this;
    }

    template <typename U>
    multiplication_proxy& operator*(const matrix_wrap<U>& m){
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(m);
        return *this;
    }

    //triggers


    operator matrix_wrap<T>(){
        std::pair<int,int> optimal_pair;
        auto result = do_multiplication(vector[0],vector[1]);
        return result;
    }


    operator matrix<T>(){
        auto result = do_multiplication(vector[0],vector[1]);
        return result;
    }

    //debug utils
    std::vector<matrix_wrap<T>> get_vector(){
        return vector;
    }

};

#endif //ASSIGNMENT2_MULTIPLICATION_PROXY_H