#include "iostream"
#include <cstddef>

#include "sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "exceptions.hpp"



template <typename T>
struct Matrix_elem{
    T elem;
    size_t i;
    size_t j;

    Matrix_elem(T new_elem = T(), size_t new_i = 0, size_t new_j = 0) : elem(new_elem), i(new_i), j(new_j){}
};



template <template <typename> class Container, typename T>
class SparseMatrix{
private:
    size_t rows;
    size_t cols;
    Container<Matrix_elem<T>> data;
public:
    SparseMatrix() : rows(0), cols(0), data(){}
    SparseMatrix(const Container<Matrix_elem<T>>& new_data, size_t new_rows, size_t new_cols) : data(new_data), rows(new_rows), cols(new_cols){}
    SparseMatrix(size_t new_rows, size_t new_col) : rows(new_rows), cols(new_col){}

    size_t Get_rows();
    size_t Get_cols();
    

    T Get(size_t i, size_t j);
    void Set(const Matrix_elem &new_elem, size_t i, size_t j);




    



};





