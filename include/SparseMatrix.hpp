#pragma once

#include "iostream"
#include <cstddef>
#include <cmath>

#include "sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "exceptions.hpp"
#include "Dict.hpp"

#include <format>


template <typename T>
struct Matrix_elem{
    T elem;
    size_t i;
    size_t j;
    Matrix_elem(T new_elem = T(), size_t new_i = 0, size_t new_j = 0) : elem(new_elem), i(new_i), j(new_j) {}
};



template <template <typename> class Container, typename T>
class SparseMatrix{
private:

    size_t rows;
    size_t cols;
    Dict<Pair<size_t, size_t>, T, Container> dict_data;

public:

    SparseMatrix();
    SparseMatrix(const Container<Matrix_elem<T>>& new_data, size_t new_rows, size_t new_cols);
    SparseMatrix(size_t new_rows, size_t new_col);


    size_t Get_rows() const;
    size_t Get_cols() const;
    size_t Get_size() const;
    T Get_elem(size_t i, size_t j) const;
    Container<Matrix_elem<T>> Get_Elements() const;


    T operator()(size_t i, size_t j) const;
    SparseMatrix operator+(const SparseMatrix& other) const;
    SparseMatrix operator*(const SparseMatrix& other) const;
    SparseMatrix operator*(const T& scalar) const;
    T Get_Norm() const;

    ~SparseMatrix();
};



//public 
template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix() : rows(0), cols(0), dict_data() {}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix(const Container<Matrix_elem<T>>& new_data, size_t new_rows, size_t new_cols) : rows(new_rows), cols(new_cols), dict_data(){
    for (auto copy : new_data){
        dict_data.Set(Pair<size_t, size_t>(copy.i, copy.j), copy.elem);
    }
}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::SparseMatrix(size_t new_rows, size_t new_cols) : rows(new_rows), cols(new_cols), dict_data() {}

template <template <typename> class Container, typename T>
size_t SparseMatrix<Container, T>::Get_rows() const{
    return rows;
}
template <template <typename> class Container, typename T>
size_t SparseMatrix<Container, T>::Get_cols() const{
    return cols;
}

template <template <typename> class Container, typename T>
size_t SparseMatrix<Container, T>::Get_size() const{
    return dict_data.Get_Items().GetLength();
}

template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::Get_elem(size_t i, size_t j) const{
    if(i >= rows || j >= cols){
        throw IndexOutOfRangeException(std::format("i до {}, j до {}. Выход из диапазона", i, j));
    }
    return dict_data.Get(Pair<size_t, size_t>(i, j));
}


template <template <typename> class Container, typename T>
Container<Matrix_elem<T>> SparseMatrix<Container, T>::Get_Elements() const{
    Container<Matrix_elem<T>> result;


    for (auto now_elem : dict_data){
        result.Append(Matrix_elem<T>(now_elem.elem2, now_elem.elem1.elem1, now_elem.elem1.elem2));
    }
    return result;
}


template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::operator()(size_t i, size_t j) const{
    return Get_elem(i, j);
}


template <template <typename> class Container, typename T>
SparseMatrix<Container, T> SparseMatrix<Container, T>::operator+(const SparseMatrix<Container, T>& other) const{
    if (rows != other.rows || cols != other.cols){
        throw InvalidSizeException(std::format("Матрицы должны иметь одинаковый размер: левая {}x {}, правая {}x {}", rows, cols, other.rows, other.cols));
    }

    Container<Matrix_elem<T>> result;
    auto real_data = Get_Elements();

    for (auto temp_get_sum : real_data){
        T sum = temp_get_sum.elem + other.Get_elem(temp_get_sum.i, temp_get_sum.j);
        if (sum != T()){
            result.Append(Matrix_elem<T>(sum, temp_get_sum.i, temp_get_sum.j));
        }
    }

    auto other_flat = other.Get_Elements();
    for (auto temp_set_elem : other_flat){
        if (Get_elem(temp_set_elem.i, temp_set_elem.j) == T()){
            result.Append(temp_set_elem);
        }
    }
    return SparseMatrix<Container, T>(result, rows, cols);
}



template <template <typename> class Container, typename T>
SparseMatrix<Container, T> SparseMatrix<Container, T>::operator*(const SparseMatrix<Container, T>& other) const{
    if (cols != other.rows){
        throw InvalidSizeException(std::format("Умножение невозможно: столбцы левой ( {}) != строки правой ( {})", cols, other.rows));
    }
    Container<Matrix_elem<T>> result;
    auto real_data = Get_Elements();

    for (size_t i = 0; i < rows; ++i){
        for (size_t j = 0; j < other.cols; ++j){
            T now_elem_sum = T();
            for (auto temp_sum : real_data){
                if (temp_sum.i == i){
                    now_elem_sum += temp_sum.elem * other.Get_elem(temp_sum.j, j);
                }
            }
            if (now_elem_sum != T()){
                result.Append(Matrix_elem<T>(now_elem_sum, i, j));
            }
        }
    }
    return SparseMatrix<Container, T>(result, rows, other.cols);
}



template <template <typename> class Container, typename T>
SparseMatrix<Container, T> SparseMatrix<Container, T>::operator*(const T& scalar) const{
    Container<Matrix_elem<T>> result;

    if (scalar != T()){
        auto real_data = Get_Elements();

        for (auto current_elem : real_data){
            T new_val = current_elem.elem * scalar;
            if (new_val != T()){
                result.Append(Matrix_elem<T>(new_val, current_elem.i, current_elem.j));
            }
        }
    }

    return SparseMatrix<Container, T>(result, rows, cols);
}


template <template <typename> class Container, typename T>
T SparseMatrix<Container, T>::Get_Norm() const{
    T sum_sq = T();
    auto real_data = Get_Elements();

    for (auto item : real_data){
        sum_sq += item.elem * item.elem;
    }

    using std::sqrt;
    return sqrt(sum_sq); 
}

template <template <typename> class Container, typename T>
SparseMatrix<Container, T>::~SparseMatrix() {}