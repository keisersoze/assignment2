//
// Created by Filippo Maganza on 19/12/2018.
//

#ifndef ASSIGNMENT2_SUM_H
#define ASSIGNMENT2_SUM_H

#include "multiplication_proxy.h"

//OVERLOADS FOR THE SUM

// matrix_wrap and matrix_ref

template<class T,class U, class dec, class dec2>
auto operator+ (const matrix_ref<T,dec>& m, const matrix_ref<U,dec2>& m2){
    if constexpr (m.is_ct() && m2.is_ct()) {
        static_assert(m.get_ct_height() == m2.get_ct_height() && m.get_ct_width() == m2.get_ct_width());
        matrix<decltype(U()+T()), m.get_ct_height(), m.get_ct_width() > result;
        for (int i = 0; i < m.get_ct_height() ; ++i) {
            for (int j = 0; j < m.get_ct_width() ; ++j) {
                result(i,j)=m(i,j)+m2(i,j);
            }
        }
        return result;
    }else {
        if (m.get_width() != m2.get_width() || m.get_height() != m2.get_height())
            throw "Size mismatch";
        matrix<decltype(U()+T())> result (m.get_height(),m.get_width());
        for (int i = 0; i < m.get_height(); ++i) {
            for (int j = 0; j < m.get_width() ; ++j) {
                result(i,j)=m(i,j)+m2(i,j);
            }
        }
        return result;
    }
}


template<class T, class U>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m, const matrix_wrap<T>& m2){
    if (m.get_width() != m2.get_width() || m.get_height() != m2.get_height())
        throw "Size mismatch";
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
    if (m.get_width() != m2.get_width() || m.get_height() != m2.get_height())
        throw "Size mismatch";
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
    return operator+(m2,m);
}

// proxy and matrix_ref

template<class T,class U, class dec, unsigned  FH>
matrix<decltype(T()+U())> operator+ ( multiplication_proxy<T,FH> m, const matrix_ref<U,dec>& m2){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

template<class T,class U, class dec, unsigned  FH, unsigned W>//
auto operator+ ( multiplication_proxy<T,FH,W> m, const matrix_ref<U,dec>& m2){
    if constexpr (FH>0) {
        matrix<T, FH, W> multiplication_result = m;
        return operator+(m2, multiplication_result);
    }else{
        matrix<T> multiplication_result = m;
        return operator+(m2, multiplication_result);
    }
}

template<class T,class U, class dec, unsigned FH>
matrix<decltype(T()+U())> operator+ (const matrix_ref<U,dec>& m2,  multiplication_proxy<T,FH> m){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

template<class T,class U, class dec, unsigned FH, unsigned W>//
auto operator+ (const matrix_ref<U,dec>& m2,  multiplication_proxy<T,FH,W> m){
    if constexpr (FH>0) {
        matrix<T, FH, W> multiplication_result = m;
        return operator+(m2, multiplication_result);
    }else{
        matrix<T> multiplication_result = m;
        return operator+(m2, multiplication_result);
    }
}

// proxy and matrix_wrap

template<class T,class U, unsigned FH>
matrix<decltype(T()+U())> operator+ ( multiplication_proxy<T,FH> m, const matrix_wrap<U>& m2){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

template<class T,class U, unsigned FH, unsigned W>
matrix<decltype(T()+U())> operator+ ( multiplication_proxy<T,FH,W> m, const matrix_wrap<U>& m2){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

template<class T,class U, unsigned FH>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m2,  multiplication_proxy<T,FH> m){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

template<class T,class U, unsigned FH, unsigned W>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m2,  multiplication_proxy<T,FH,W> m){
    matrix<T> multiplication_result = m;
    return operator+(m2,multiplication_result);
}

//two proxies

template<class T,class U, unsigned FH, unsigned FH2>
matrix<decltype(T()+U())> operator+ (multiplication_proxy<T,FH> m2,  multiplication_proxy<U,FH2> m){
    matrix<T> multiplication_result_1 = m;
    matrix<T> multiplication_result_2 = m2;
    return operator+(multiplication_result_1,multiplication_result_2);
}

template<class T,class U, unsigned FH, unsigned FH2, unsigned W>
matrix<decltype(T()+U())> operator+ (multiplication_proxy<T,FH,W> m2,  multiplication_proxy<U,FH2> m){
    matrix<T> multiplication_result_1 = m;
    matrix<T> multiplication_result_2 = m2;
    return operator+(multiplication_result_1,multiplication_result_2);
}

template<class T,class U, unsigned FH, unsigned FH2, unsigned W>
matrix<decltype(T()+U())> operator+ (multiplication_proxy<T,FH> m2,  multiplication_proxy<U,FH2, W> m){
    matrix<T> multiplication_result_1 = m;
    matrix<T> multiplication_result_2 = m2;
    return operator+(multiplication_result_1,multiplication_result_2);
}

template<class T, class U, unsigned FH, unsigned FH2, unsigned W, unsigned W2>
auto operator+ (multiplication_proxy<T,FH, W> m2,  multiplication_proxy<U,FH2, W2> m){
    if constexpr (W>0 && W2 >0) {
        matrix<T,FH,W> multiplication_result_1 = m;
        matrix<T,FH2,W2> multiplication_result_2 = m2;
        return operator+(multiplication_result_1, multiplication_result_2);
    }else {
        matrix<T> multiplication_result_1 = m;
        matrix<T> multiplication_result_2 = m2;
        return operator+(multiplication_result_1, multiplication_result_2);
    }
}

#endif //ASSIGNMENT2_SUM_H
