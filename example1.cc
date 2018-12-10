#include<iostream>

#include"matrix.h"


int main() {

matrix<int,4,5> A;


for (int i=0; i!=4; ++i)
	for(int j=0; j!=5; ++j)
	  A(i,j) = i*10+j;

//std::cout << A(1,2) << std::endl;

for (int i=0; i!=4; ++i) {
	for(int j=0; j!=5; ++j)
		std::cout << A(i,j) << ' ';
	std::cout << '\n';
}
std::cout << std::endl;


auto B=A.transpose();


std::cout << B(1,2) << ' ' << *(B.begin()+1) << std::endl;
for (int i=0; i!=5; ++i) {
	for(int j=0; j!=4; ++j)
		std::cout << B(i,j) << ' ';
	std::cout << '\n';
}
std::cout << std::endl;


auto C=B.window<1,3,2,3>();
auto D=C.transpose();
std::cout << C(0,0) <<"\n";


for (auto iter=C.begin(); iter != C.end(); ++iter)
	std::cout << *iter << ' ';
std::cout << "\n\n";


/*
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
