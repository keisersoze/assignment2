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
matrix<decltype(R()*U())> do_multiplication(const matrix_wrap<R>& m1,const matrix_wrap<U>& m2){
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
    return result;
}

template< class R, unsigned H, unsigned W>
matrix<R,H,W> do_multiplication_ct_dim(const matrix_wrap<R>& m1,const matrix_wrap<R>& m2){
    matrix<R,H,W> result;
    if (m1.get_width() != m2.get_height())
        throw "!!!";
    for (int k = 0; k < m1.get_height() ; ++k) {
        R buff(0);
        for (int i = 0; i < m2.get_width() ; ++i) {
            for (int j = 0; j < m1.get_width(); ++j) {
                buff += m1(k,j) * m2 (j,i);
            }
            result(k,i)=buff;
            buff=0;
        }
    }
    return result;
}

template<typename T, unsigned...dims> class multiplication_proxy;

template <typename T, unsigned FH>
class multiplication_proxy <T,FH> {
protected:
    std::vector<matrix_wrap<T>> vector;

    /* Find the current optimal multiplication (greedy largest-dimension compression heuristic), no side effects
     * Pre: the size of the vector must be >= 2
     * Time complexity: O(n), with n equal to the size of the vector
     */
    inline unsigned find_optimal_multiplication () const{
        unsigned current_opt = vector[0].get_width();
        unsigned opt_idx = 0;
        for (unsigned i = 0; i < vector.size() - 1 ; ++i) {
            unsigned current_size = vector[i].get_width();
            if (current_opt < current_size ) {
               current_opt = current_size;
               opt_idx = i;
            }
        }
        return opt_idx;
    }
public:
    multiplication_proxy<T,FH>()= default;
    multiplication_proxy<T,FH>(multiplication_proxy<T,FH> &&m):vector(std::move(m.vector)){}
    multiplication_proxy<T,FH>(const multiplication_proxy<T,FH> &m):vector(m.vector){}

    template < class dec>
    void add (const matrix_ref<T,dec>& m){
        vector.push_back(m);
    }

    void add (const matrix_wrap<T> &m){
        vector.push_back(m);
    }

    template<class dec>
    multiplication_proxy<T,FH> &operator*(const matrix_ref<T, dec> &m) {
        if (vector.back().get_width() != m.get_height())
            throw "Size mismatch";
        vector.push_back(matrix_wrap<int>(m));
        return *this;
    }

    multiplication_proxy<T,FH> &operator*(const matrix_wrap<T> &m) {
        if (vector.back().get_width() != m.get_height())
            throw "Size mismatch";
        vector.push_back(m);
        return *this;
    }

    operator matrix<T>(){
        //BEGIN DEBUG
        for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());
        std::cout<< "----------------\n";
        //END DEBUG

        while(vector.size() > 2) {
            unsigned opt = find_optimal_multiplication();
            matrix<T> result = do_multiplication(vector[opt], vector[opt + 1]);
            vector.erase(vector.begin() + opt);
            vector[opt]= result;

            //BEGIN DEBUG
            for (auto i=vector.begin(); i<vector.end(); ++i)
                pprint_wrap(*i,(*i).get_height(),(*i).get_width());
            std::cout<< "----------------\n";
            //END DEBUG
        }
        return do_multiplication(vector[0],vector[1]);
    }
};

template <typename T, unsigned FH, unsigned W>
class multiplication_proxy <T, FH, W> : public multiplication_proxy<T,FH>{
private:
    using multiplication_proxy<T,FH>::vector;
    using multiplication_proxy<T,FH>::find_optimal_multiplication;
public:
    multiplication_proxy<T,FH,W>()= default;
    multiplication_proxy<T,FH,W>(multiplication_proxy<T,FH> &&m):multiplication_proxy<T,FH>(std::move(m)){}
    multiplication_proxy<T,FH,W>(const multiplication_proxy<T,FH> &m):multiplication_proxy<T,FH>(m){}

    template<class dec>
    auto operator*(const matrix_ref<T, dec> &m) {
        if constexpr (m.is_ct()) {
            static_assert(m.get_ct_height() == W);
            multiplication_proxy<T,FH,m.get_ct_width()> result(std::move(*this));
            result.add(m);
            return result;
        }else{
            if (vector.back().get_width() != m.get_height())
                throw "Size mismatch";
            vector.push_back(m);
            return *this;
        }
    }

    operator matrix<T, FH, W>(){
        static_assert(FH>0 && W>0);
        //BEGIN DEBUG
        for (auto i=vector.begin(); i<vector.end(); ++i)
            pprint_wrap(*i,(*i).get_height(),(*i).get_width());
        std::cout<< "----------------\n";
        //END DEBUG

        while(vector.size() > 2) {
            unsigned opt = find_optimal_multiplication();
            matrix<T> result = do_multiplication(vector[opt], vector[opt + 1]);
            vector.erase(vector.begin() + opt);
            vector[opt]= result;

            //BEGIN DEBUG
            for (auto i=vector.begin(); i<vector.end(); ++i)
                pprint_wrap(*i,(*i).get_height(),(*i).get_width());
            std::cout<< "----------------\n";
            //END DEBUG
        }
        return do_multiplication_ct_dim<T,FH,W>(vector[0],vector[1]);

    }

};

//OVERLOADS FOR THE MULTIPLICATION

template<class T, class dec, class dec2>
auto operator* (const matrix_ref<T,dec>& m, const matrix_ref<T,dec2>& m2){
    if constexpr (m.is_ct() && m2.is_ct()) {
        static_assert(m.get_ct_width()==m2.get_ct_height());
        multiplication_proxy<T, m.get_ct_height(), m2.get_ct_width()> result;
        result.add(m);
        result.add(m2);
        return result;
    }else{
        if (m.get_width()!=m2.get_height())
            throw "Size mismatch";
        if constexpr (m.is_ct()) {
            multiplication_proxy<T, m.get_ct_height()> result;
            result.add(m);
            result.add(m2);
            return result;
        }else{
            multiplication_proxy<T,0> result;
            result.add(m);
            result.add(m2);
            return result;
        }
    }
}

template<class T,unsigned FH>
multiplication_proxy<T,FH> operator* (const matrix_wrap<T>& m, const matrix_wrap<T>& m2){
    if (m.get_width()!=m2.get_height())
        throw "Size mismatch";
    else{
        multiplication_proxy<T,0> result;
        result.add(m);
        result.add(m2);
        return result;
    }
}

template<class T, class dec, unsigned FH>
multiplication_proxy<T,FH> operator* (const matrix_ref<T,dec>& m, const matrix_wrap<T>& m2){
    if (m.get_width()!=m2.get_height())
        throw "Size mismatch";
    else{
        if constexpr (m.is_ct()) {
            multiplication_proxy<T, m.get_ct_height()> result;
            result.add(m);
            result.add(m2);
            return result;
        }else{
            multiplication_proxy<T,0> result;
            result.add(m);
            result.add(m2);
            return result;
        }
    }
}

template<class T, class dec, unsigned FH>
multiplication_proxy<T,FH> operator* ( const matrix_wrap<T>& m, const matrix_ref<T,dec>& m2){
    if (m.get_width()!=m2.get_height())
        throw "Size mismatch";
    else{
        multiplication_proxy<T,0> result;
        result.add(m);
        result.add(m2);
        return result;
    }
}


#endif //ASSIGNMENT2_MULTIPLICATION_PROXY_H