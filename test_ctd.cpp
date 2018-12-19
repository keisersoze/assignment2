#include <iostream>
#include <string>
#include "matrix.h"

#define H 5
#define W 6

template<typename T>
void pprint(T mat, std::string msg) {
	std::cout << "----------------------------------------" << std::endl;
	std::cout << msg << "\n\n";
	for (int i = 0; i < mat.get_height(); ++i) {
		for (int j = 0; j < mat.get_width(); ++j) {
			std::cout << mat(i,j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, char **argv) {

	std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
	std::cout << "| COMPILE-TIME MATRIX DEMO  |" << std::endl;
	std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
	matrix<int,H,W> A;

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			A(i,j) = i * W + j;
		}
	}
	pprint(A, "base matrix");
	std::cout << "const size is: " << decltype(A)::get_ct_height() << " x " << decltype(A)::get_ct_width() << std::endl;

	auto B = A.transpose();
	pprint(B, "transpose()");
	std::cout << "const size is: " << B.get_ct_height() << " x " << decltype(B)::get_ct_width() << std::endl;

	auto C = B.transpose();
	pprint(C, "transpose()");
	std::cout << "const size is: " << decltype(C)::get_ct_height() << " x " << decltype(C)::get_ct_width() << std::endl;

	auto D = C.window({1, 4, 2, 5});
	pprint(D, "window({1, 4, 2, 5})");
	std::cout << "const size is: " << decltype(D)::get_ct_height() << " x " << decltype(D)::get_ct_width() << std::endl;
	std::cout << "windowing does not allow to maintain" << std::endl;
	std::cout << "compile-time knowledge on dimensions" << std::endl;

	auto E = D.diagonal();
	pprint(E, "diagonal()");
	std::cout << "const size is: " << decltype(E)::get_ct_height() << " x " << decltype(E)::get_ct_width() << std::endl;
	std::cout << "const size is lost!" << std::endl;

	auto F = E.diagonal_matrix();
	pprint(F, "diagonal_matrix()");
	std::cout << "const size is: " << decltype(F)::get_ct_height() << " x " << decltype(F)::get_ct_width() << std::endl;
	std::cout << "const size is lost!" << std::endl;

	std::cout << std::endl;



	std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
	std::cout << "| TEMPLATE GET<i,j>() DEMO  |" << std::endl;
	std::cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << std::endl;
	
	matrix<int,2,2> M;

	M.get<0,0>() = 3;
	M.get<0,1>() = 2;
	M.get<1,0>() = 1;
	M.get<1,1>() = 5;

	int det = M.get<0,0>() * M.get<1,1>() - M.get<1,0>() * M.get<0,1>();

	pprint(M, "the determinant of M");

	std::cout << "is: " << det << std::endl;

	auto N = M.transpose();

	N.get<0,0>() = -1;
	N.get<0,1>() = 7;
	N.get<1,0>() = 3;
	N.get<1,1>() = 3;

	det = N.get<0,0>() * N.get<1,1>() - N.get<1,0>() * N.get<0,1>();

	pprint(N, "the determinant of N");

	std::cout << "is: " << det << std::endl;
}