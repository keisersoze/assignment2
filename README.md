Filippo Maganza (858532) and Sebastiano Smaniotto (857744)

Assignment 2: Matrix Operations

Due date: 19/12/2018 

## REQUIREMENT SPECIFICATION

Extend the matrix library (either the one presented or, if you feel confident and lucky, your own) to include the following features:

1. Compile-time matrix dimensions: provide a variant of the matrix that maintains compile-time information about height and width. Allow accessors to be bounds-checked at compile time and maintain compile-time size information as much as possible through matrix operators.

2. Matrix operations: provide matrix addition and (row-by-column) multiplication. Operations should interact with compile-time dimensions correctly (dimensions checked at compile-time).

When dealing with multiple multiplications, the library must perform the sequence of multiplications in the most computationally efficient order, i.e., by multiplying matrices contracting the largest dimension first. For example, let A, B, and C be 2x3, 3x5, and 5x2 matrices respectively, then the expression A*B*C should be computed as if it where A*(B*C), performing first the multiplication B*C since it is contracting the dimension 5, and then multiplying A by the result, contracting the smaller dimension 3.

## USAGE

The client is now able to use a specialized version of the `matrix<E>` class which takes two other parameter `matrix<E,H,W>` in the template which represents the height and the width of the matrix. By doing so the static information will be maintained throughout the templated decorator chain until the method `window(...)` will be invoked. The absence of a static window class does not allow to establish the matrix dimension at compile-time, and so the compile-time knowledge is lost.

Since the matrix specialization does not depend on a change in the template structure of `matrix_ref<T,type>` (being `type` both the placeholder for the type and the type decoration) then `matrix_wrap<E>` is oblivious to any change in the static variant, meaning that no change has been necessary and the client is free to pass a static `matrix_ref<T,type>` to any `matrix_wrap<E>` as in the previous assignment.

Operators for matrix sum and multiplication are now available. Any expression containing these operators is evaluated as a `matrix_wrap<E>`. Their behaviour is described in their own Design section.

Below are presented all the operation offered by `matrix <E>` and `shared_matrix  <E>` classes.

### Examples

*TODO*
```

## DESIGN

In this section we explain our design for the project.
 
### Static matrices

We have defined a new generic template for the `matrix` class so as to be able to create a static specialization by allowing to the client to specify the dimensions of the matrix directly in the template, and added a new policy `CT_matrix<H,W>` to provide a variant to the `Plain` base `matrix_ref<T,type>` templated class where we can store static information at compile-time on the given matrix size. Such information is carried through the static decoration chain by copying over the value in the two `static constexpr unsigned h, w` variables. Since no static variant to `window_spec` is provided, the static information on the dimensions is lost when the method `window(window_spec w)` is invoked and the values of `h` and `w` are set to zero. Using this information, we can check at compile-time wether the new static getters shall perform a bound check or delegate the access to the `operator()`.

The `matrix_wrap<E>` required no changes since the type knowledge on the wrapped matrix is restricted to the templated class `matrix_ref<E,type>` and so the wrapper is oblivious to the new features.

### Operators 

All operators have been defined externally from the classes. We have provided an overload for each possible combination of type of matrix (`matrix<E, ...>`, `matrix_ref<E,type>`, `matrix_wrap<E>`). 

The `operator*` allows the client to multiply two matrices having the same type. A sequence of multiplication produces a `multiplication_proxy<E>` object which stores all the matrices converting them into a `matrix_wrap<E>` and the quasi-optimal multiplication order is decided at runtime using the largest-dimension compression heuristic. 

The sum can be performed using the `operator+` between any two matrices of type `T` and `U`, as long as the `operator+` is defined between two expressions having those two types, and returns a new matrix having as element type the type that one would get by summing two expressions of type `T` and `U`.

