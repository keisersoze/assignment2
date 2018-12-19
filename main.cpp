#include <iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"
#include "sum.h"
//#include "printer.h"

#define SIZE 4
#define TYPE matrix<int,SIZE,SIZE>

int main( int argc, char *argv[] ) {
	TYPE A, B;

	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			A(i,j) = i * SIZE + j;
			B(i,j) = 2 * i - i - j + 1;
		}
	}

	matrix<int> res = A * A.transpose() + B;
	pprint(res);
}