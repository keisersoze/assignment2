Filippo Maganza (858532) and Arnaldo Santoro (822274)

Assignment 1: Matrix Library

Due date: 27/11/2018 

## REQUIREMENT SPECIFICATION

Implement a templated library handling matrices, vectors, and covectors; where vectors are nx1 matrices and covectors are 1xn matrices.

the library must offer operation like:

* submatrix: return a matrix that contains a contiguous subrange of rows and columns of the original matrix
* transpose: return a matrix with rows and columns inverted
* diagonal: return a vector containing the diagonal elements of a given matrix
* diagonalmatrix: return an unmodifiable diagonal matrix whose diagonal elements are those of a given vector

The matrices returned by such operators must share the data with the original matrix, but direct copy of the matrices and vectors must result in a deep copy (sharing a matrix can be obtained by taking a submatrix extending the whole row and column ranges).

Create forward iterators for the matrix traversing it either in row-major or column-major order. Any matrix should be traversable in either direction regardless of its natural representation order. Any given iterator will traverse only in a given order.

## USAGE

The client code have to manage two different templated classes: `matrix <E>` and its derived class `shared_matrix  <E>`. The only difference is that the formers's copy construct does a deep copy of the data while the latter's copy constructor return a matrix that share data with the original one.

Below are presented all the operation offered by `matrix <E>` and `shared_matrix  <E>` classes.

``` c++

    E operator ()( int accessed_row, int accessed_column) const;

    E& operator ()(int accessed_row, int accessed_column);

    E getRowNumber() const;

    E getColumnNumber() const;

    shared_matrix <E> transpose() const;

    shared_matrix <E> diagonal() const;

    shared_matrix <E> submatrix( int first_row, int first_col, int last_row ,int last_col ) const;

    shared_matrix <E> submatrix(pair <int, int > first_pair, pair <int ,int > second_pair) const;

    const shared_matrix <E> diagonal_matrix() const;

    row_matrix_iterator <E> begin() const;

    row_matrix_iterator <E> end() const;

    column_matrix_iterator <E> begin() const;

    column_matrix_iterator <E> end() const;
    
```
### Examples

Initialization a new matrix:

``` c++
std::vector < int > elements_vector_a = {1, 2, 3, 4, 5, 6};
matrix < int > int_matrix (elements_vector_a, 2, 3);

std::vector < double > elements_vector_b = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
matrix < double > double_matrix(elements_vector_b, 3, 2);

std::vector < char > elements_vector_c = {'a', 'b', 'd', 'g', 'e', 'g', '', '4', ']'};
matrix < char > char_matrix(elements_vector_c, 3, 3);

matrix < int > error (elements_vector_a, 0, 3); // error: invalid input row
matrix < int > error (elements_vector_a, 2, -1); // error: invalid input column
matrix < int > error (elements_vector_a, 3, 3); // error: invalid input vector

double_matrix.getRowNumber() == 3; // true
double_matrix.getColumnNumber() == 2; // true
```

Access elements of a matrix:

``` c++
double a_double = double_matrix(accessed_row,accessed_column);
```

Assign values to elements:

``` c++
int value;
m(row,column) = value;
```

Copy a matrix:
```
matrix<int> copied_matrix = int_matrix; //Deep copy -> data are NOT shared
auto copied_matrix = double_matrix.transpose(); //Copy with shared data

const auto copied_matrix = int_matrix.diagonal_matrix(); 
// note the use of const to avoid accessing a const reference
```

Obtain a transposed a matrix:
``` c++
double a_double = double_matrix.transpose()
```

Obtain a vector of diagonal elements of a matrix:
``` c++
int_matrix.diagonal();
```

Obtain a const diagonal matrix:
``` c++
int row = 1, column = 2;
const auto constant_diagonal_matrix int_matrix.diagonal_matrix(); //note the use of const

constant_diagonal_matrix(row,column) = 666; // error: unmodifiable matrix
```

Obtain a submatrix:
``` c++
auto sub_matrix_a = char_matrix.submatrix( 2,2, 3,3); 
// obtain a matrix which is a 2x2 submatrix with the element of the decorated matrix in position 2,2 as first element and the element at position (3,3) as the last element.

pair<int,int> first_pair = (1,1);
pair<int,int> last_pair = (2,3);
auto sub_matrix_b = char_matrix.submatrix( first_pair, last_pair); 
// two methods to obtain a submatrix were coded to show a possible, clearer, alternative.
// the authors would prefer to keep only a method for semplicity and clarity but the example was interesting enough to be kept till the last hour, and it remained in the last version.
```

Use matrix iterator methods `begin()` and `end()`:
``` c++
auto begin = matrix.begin(); // returns the first element of the matrix.
auto begin = matrix.end(); // Actual use is for cycles, as it returns the first past-the-end element of the matrix.
```

## DESIGN

In this section we explain our design for the project.
 
### Decorator design pattern

Our matrix template has been designed with the decorator pattern. In particular we have implemented this pattern using the dynamic polymorphism offered by c++ :

* `matrix_impl <E>` is the interface for the base matrix class and the decorator classes. 

* `base_matrix_impl <E>` own the concrete data of the matrix. Has two integers r and c that represent row and column numbers, and a vector data, of type E, which contains the elements of the matrix. 

* `transposed_matrix_impl <E>`
        the decoration class for the transposed matrix;
        it overrides the accessors calling the same methods to the decorated matrix with inverted input. 

* `diagonal_impl <E>`
        the decoration class for the vector which contains the diagonal elements of the decorated matrix;
        it is a vector (a matrix with r rows and 1 column) accessing elements of the decorated matrix which are on diagonal positions; note that its rows are the minimum between rows and columns of the decorated matrix.
    
* `diagonal_matrix_impl <E>`
        the decoration class for the unmodifiable matrix which returns 0's on non-diagonal positions;
        it override accessors returning the 0 element of the type `<E>` in the non-diagonal positions.

* `submatrix_matrix_impl <E>`
        the decoration class for a submatrix. The submatrix is modeled with two pair of int that represent the first and last elements of the submatrix. The class overrides accessors performing additional controls based on the size of the submatrix.
        
The pros and cons of this solution are:

* Pros: 
    * Simplicity
    * Flexibility (it is possible to add new decorators to support new operations )
    * Easy to mantain 
    * Well known design pattern
* Cons: 
    * The use of a decorator design pattern with dynamic polymorphism implies the possibility of a long chain of calls thus causing a deterioration of the performance. It may be wise to make use the deep copies of matrices with a long chain of calls when possible.

### Shared pointers
In our design we decided to use smart ponters over raw pointers to manage lifetime of the objects, thus preventing memory leaks from unexpected behaviours from user's program.

Once we decided to use smart pointers we discussed the advantages / disadvantages between two possible implementations: `std::shared_ptr` and `std::unique_ptr`.

We have choosen to use `std::shared_ptr` because in our design the objects of the decorator chain could have multiple wrapper objects ( e.g. a `matrix` object and a decorator object).

### PIMPL idiom
The PIMPL Idiom (Pointer to IMPLementation) is a technique for implementation hiding in which a public class wraps a structure or class that cannot be seen outside the library the public class is part of.

We adopted PIMPL to hide the management of the smart pointers inside the wrapper class `matrix` thus offering a better usability and security (user can never directly access pointers).


### Iterators
We implemented two forward iterators: row iterator and column iterator.
They have a shared pointer to a `matrix_impl <E>` and two integer for the current row and columns. 
They overload the operators:

``` c++
row_matrix_iterator& operator ++();

row_matrix_iterator& operator ++(int);

const E& operator *();

bool operator == (const row_matrix_iterator& x);

bool operator != (const row_matrix_iterator& x);

```

### The zero element
We used a templated function to return the zero element of a given type. For numerical types we implemented a specialization of the template that return the correct value instead of a null pointer.

