#include<iostream>

#include"matrix.h"

template<typename T, unsigned R, unsigned C>
void pprint(T matrix){
	for (int i = 0; i < C; ++i) {
		for (int j = 0; j < R ; ++j) {
			std::cout << matrix(i,j) << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

int main() {

matrix<int,4,5> A;


for (int i=0; i!=4; ++i)
	for(int j=0; j!=5; ++j)
	    A(i,j) = 10+ i*10+j;

auto B=A.transpose();
auto C=B.window<1,3,2,3>();
auto D=C.transpose();

std::cout << A.get_height()<< ' ' << A.get_width() << '\n';
std::cout << B.get_height()<< ' ' << B.get_width() << '\n';
std::cout << C.get_height()<< ' ' << C.get_width() << '\n';
std::cout << D.get_height()<< ' ' << D.get_width() << '\n';

pprint<decltype(A),A.get_width(),A.get_height()>(A);
pprint<decltype(B),B.get_width(),B.get_height()>(B);
pprint<decltype(C),C.get_width(),C.get_height()>(C);
pprint<decltype(D),D.get_width(),D.get_height()>(D);

/*
auto E=B.diagonal_matrix();
std::cout << E.get_height()<< ' ' << E.get_width() << '\n';
pprint<decltype(E),E.get_width(),E.get_height()>(E);*/


/*
for (auto iter=C.begin(); iter != C.end(); ++iter)
	std::cout << *iter << ' ';
std::cout << "\n\n";



auto D=B.diagonal();
std::cout << D(1) << '=' << D.get_height() << 'x' << D.get_width() <<  std::endl;

for (auto iter=D.begin(); iter != D.end(); ++iter)
	std::cout << *iter << ' ';
std::cout << "\n\n";

auto E=D.diagonal_matrix();
std::cout << E(1,2) << ' ' << E(2,2) << std::endl;

for (auto iter=E.row_begin(1); iter != E.row_end(1); ++iter)
	std::cout << *iter << ' ';
std::cout << "\n\n============================\n\n";


matrix<int> BC=B;

B(1,2)=0;

for (int i=0; i!=5; ++i) {
	for(int j=0; j!=4; ++j)
		std::cout << B(i,j) << ' ';
	std::cout << '\n';
}
std::cout << std::endl;

for (int i=0; i!=5; ++i) {
	for(int j=0; j!=4; ++j)
		std::cout << BC(i,j) << ' ';
	std::cout << '\n';
}
std::cout << std::endl;

for (int i=0; i!=4; ++i) {
	for(int j=0; j!=5; ++j)
		std::cout << A(i,j) << ' ';
	std::cout << '\n';
}
std::cout << std::endl;
 */
return 0;
}
