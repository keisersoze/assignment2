//
// Created by Filippo Maganza on 13/12/2018.
//

#ifndef ASSIGNMENT2_MULTIPLICATION_PROXY_H
#define ASSIGNMENT2_MULTIPLICATION_PROXY_H

#include<vector>

//SUM
/*
template<class U, class decorated>
matrix<decltype(T()+U())> operator+ (const matrix_ref<U,decorated>& m) const{
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+matrix_ref<T,Plain>::operator()(i,j);
        }
    }
    return result;
}


template<class U>
matrix<decltype(T()+U())> operator+ (const matrix_wrap<U>& m) const{
    matrix<decltype(U()+T())> result(m.get_height(),m.get_width());
    for (int i = 0; i < m.get_height(); ++i) {
        for (int j = 0; j < m.get_width() ; ++j) {
            result(i,j)=m(i,j)+matrix_ref<T,Plain>::operator()(i,j);
        }
    }
    return result;
}


//PRODUCT

template<class U>
multiplication_proxy<decltype(T()+U())> operator* (const matrix<U>& m) const{
    multiplication_proxy<decltype(T()+U())> result;
    result.operator*(m);
    result.operator*(matrix_wrap <T> (*this));
    int i = result.get_vector().size();
    return result;
}*/


template<typename T>
class matrix_wrap;

template<typename T>
class matrix;

template<typename T, class decorated>
class matrix_ref;

//FUNCTIONS

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

    //std::pair<int,int> find_optimal_

public:

    multiplication_proxy& operator*(const matrix<T>& m){
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(m);
        return *this;
    }

    multiplication_proxy& operator*(const matrix_wrap<T>& m){
        if (!vector.empty()) {
            if (vector.back().get_width() != m.get_height())
                throw "!!!";
        }
        vector.push_back(m);
        return *this;
    }

    operator matrix_wrap<T>(){
        auto result = do_multiplication(vector[0],vector[1]);
        return result;
    }

    operator matrix<T>(){
        auto result = do_multiplication(vector[0],vector[1]);
        return result;
    }

    std::vector<matrix_wrap<T>> get_vector(){
        return vector;
    }

};

#endif //ASSIGNMENT2_MULTIPLICATION_PROXY_H