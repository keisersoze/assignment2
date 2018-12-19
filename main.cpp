#include <iostream>

#include "matrix.h"
#include "matrix_wrap.h"
#include "multiplication_proxy.h"
#include "printer.h"
#include "sum.h"

int main() {
	matrix<int> A(4,4);
	matrix<int> B(4,4);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			A(i,j) = i * 4 + j;
			B(i,j) = 2 * i - i - j + 1;
		}
	}

	matrix<int> res = A * B + B + B;

	pprint(res);
	return 0;
}