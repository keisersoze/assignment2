#ifndef _MATRIX_FWD_H_
#define _MATRIX_FWD_H_


template <unsigned R, unsigned C> struct Plain;
template<class decorated> struct Transpose;
template<class decorated, unsigned R1, unsigned C1, unsigned R2, unsigned C2> struct Window;
template<class decorated> struct Diagonal;
template<class decorated> struct Diagonal_matrix;

template<typename T, class matrix_type=Plain<0,0>> class matrix_ref;

#endif //_MATRIX_FWD_H_
