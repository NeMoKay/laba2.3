#include "iostream"
#include <cstddef>

#include "sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "exceptions.hpp"

#include <format>



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
    SparseMatrix();
    SparseMatrix(const Container<Matrix_elem<T>>& new_data, size_t new_rows, size_t new_cols);
    SparseMatrix(size_t new_rows, size_t new_col);


    size_t Get_rows() const;
    size_t Get_cols() const;
    T Get_elem(size_t i, size_t j) const;

    

    T operator()(size_t i, size_t j) const;
    SparseMatrix operator+(const SparseMatrix& other) const;
    SparseMatrix operator*(const SparseMatrix& other) const;
    T Get_Norm() const;

    ~SparseMatrix();
};



//public 
template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix() : rows(0), cols(0), data() {}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix(const Container<Matrix_elem<T>>& new_data, size_t new_rows, size_t new_cols) : rows(new_rows), cols(new_cols), data(new_data) {}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix(size_t new_rows, size_t new_cols) : rows(new_rows), cols(new_cols), data() {}


template <template <typename> class Container, typename T>
size_t SparseMatrix<Container, T>::Get_rows() const{
    return rows;
}
template <template <typename> class Container, typename T>
size_t SparseMatrix<Container, T>::Get_cols() const{
    return cols;
}

template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::Get_elem(size_t i, size_t j) const{
    if(i >= rows || j >= cols){
        throw IndexOutOfRangeException(std::format("i до {}, j до {}. Выход из диапазона", i, j));
    }

    for (size_t k = 0; k < data.GetLength(); ++k) {
        Matrix_elem<T> copy = data.Get(k);
        if (copy.i == i && copy.j == j){
            return copy.elem;
        }
    }

    return T();
}

template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::operator()(size_t i, size_t j) const{
    return Get_elem(i, j);

}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T> SparseMatrix<Container, T>::operator+(const SparseMatrix<Container, T>& other) const {
    if (rows != other.rows || cols != other.cols){
        throw InvalidSizeException(std::format("Матрицы должны иметь одинаковый размер: левая {}x{}, правая {}x{}", rows, cols, other.rows, other.cols));
    }

    Container<Matrix_elem<T>> res_data;

    for (size_t k = 0; k < data.GetLength(); ++k){
        Matrix_elem<T> temp_get_sum = data.Get(k);

        T sum = temp_get_sum.elem + other.Get_elem(temp_get_sum.i, temp_get_sum.j);
        if (sum != T()){
            res_data.Append(Matrix_elem<T>(sum, temp_get_sum.i, temp_get_sum.j));
        }
    }


    for (size_t k = 0; k < other.data.GetLength(); ++k){
        Matrix_elem<T> temp_set_elem = other.data.Get(k);

        if (Get_elem(temp_set_elem.i, temp_set_elem.j) == T()){
            res_data.Append(temp_set_elem);
        }
    }
    return SparseMatrix<Container, T>(res_data, rows, cols);
}


template <template <typename> class Container, typename T>
SparseMatrix<Container, T> SparseMatrix<Container, T>::operator*(const SparseMatrix<Container, T>& other) const {
    if (cols != other.rows){
        throw InvalidSizeException(std::format("Умножение невозможно: столбцы левой ({}) != строки правой ({})", cols, other.rows));
    }
    Container<Matrix_elem<T>> res_data;

    for (size_t i = 0; i < rows; ++i){
        for (size_t j = 0; j < other.cols; ++j){
            T current_sum = T();
            for (size_t k = 0; k < data.GetLength(); ++k){
                Matrix_elem<T> temp_sum = data.Get(k);
                if (temp_sum.i == i){
                    current_sum += temp_sum.elem * other.Get_elem(temp_sum.j, j);
                }
            }
            if (current_sum != T()){
                res_data.Append(Matrix_elem<T>(current_sum, i, j));
            }
        }
    }
    return SparseMatrix<Container, T>(res_data, rows, other.cols);
}


template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::Get_Norm() const{
    T sum_sq = T();
    for (size_t k = 0; k < data.GetLength(); ++k){
        T val = data.Get(k).elem;
        sum_sq += val * val;
    }

    using std::sqrt;
    return sqrt(sum_sq); 
}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::~SparseMatrix() {}