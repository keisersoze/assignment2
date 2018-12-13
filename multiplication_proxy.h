//
// Created by Filippo Maganza on 13/12/2018.
//

#ifndef ASSIGNMENT2_MULTIPLICATION_PROXY_H
#define ASSIGNMENT2_MULTIPLICATION_PROXY_H

#include<vector>

template<typename T>
class matrix_wrap;

template<typename T>
class matrix;

template <class T>
class multiplication_proxy{
private:
    std::vector<matrix_wrap<T>> vector;

    template< class R, class U>
    matrix<decltype(R()+U())> do_multiplication(const matrix_wrap<R>& m1,const matrix_wrap<U>& m2) const{
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