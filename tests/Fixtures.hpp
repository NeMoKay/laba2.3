#include <gtest/gtest.h>

#include "SparseMatrix.hpp"
#include "Complex.hpp"

#include "Dict.hpp"
#include "ArraySequence.hpp"

class Sparse_Matrix_Fixture : public testing::Test{
protected:
    SparseMatrix<ArraySequence, int>* empty_mat;
    SparseMatrix<ArraySequence, int>* zero_mat;
    
    SparseMatrix<ArraySequence, int>* mat_A;
    SparseMatrix<ArraySequence, int>* mat_B;
    SparseMatrix<ArraySequence, int>* mat_C;

    SparseMatrix<ArraySequence, Complex>* mat_complex_A;
    SparseMatrix<ArraySequence, Complex>* mat_complex_B;

    void SetUp() override{
        empty_mat = new SparseMatrix<ArraySequence, int>;
        zero_mat = new SparseMatrix<ArraySequence, int>(3, 3);

        ArraySequence<Matrix_elem<int>> seq_A;
        seq_A.Append(Matrix_elem<int>(1, 0, 0));
        seq_A.Append(Matrix_elem<int>(2, 0, 2));
        seq_A.Append(Matrix_elem<int>(3, 1, 1));
        seq_A.Append(Matrix_elem<int>(4, 2, 0));
        seq_A.Append(Matrix_elem<int>(5, 2, 2));
        mat_A = new SparseMatrix<ArraySequence, int>(seq_A, 3, 3);

        ArraySequence<Matrix_elem<int>> seq_B;
        seq_B.Append(Matrix_elem<int>(2, 0, 2));
        seq_B.Append(Matrix_elem<int>(1, 1, 0));
        seq_B.Append(Matrix_elem<int>(2, 1, 1));
        seq_B.Append(Matrix_elem<int>(-5, 2, 2));
        mat_B = new SparseMatrix<ArraySequence, int>(seq_B, 3, 3);

        ArraySequence<Matrix_elem<int>> seq_C;
        seq_C.Append(Matrix_elem<int>(1, 0, 0));
        seq_C.Append(Matrix_elem<int>(2, 0, 1));
        seq_C.Append(Matrix_elem<int>(3, 1, 1));
        seq_C.Append(Matrix_elem<int>(4, 2, 0));
        mat_C = new SparseMatrix<ArraySequence, int>(seq_C, 3, 2);

        ArraySequence<Matrix_elem<Complex>> seq_comp_A;
        seq_comp_A.Append(Matrix_elem<Complex>(Complex(1.0, 2.0), 0, 0));
        seq_comp_A.Append(Matrix_elem<Complex>(Complex(3.0, -1.0), 1, 1));
        mat_complex_A = new SparseMatrix<ArraySequence, Complex>(seq_comp_A, 2, 2);

        ArraySequence<Matrix_elem<Complex>> seq_comp_B;
        seq_comp_B.Append(Matrix_elem<Complex>(Complex(2.0, 1.0), 0, 0));
        seq_comp_B.Append(Matrix_elem<Complex>(Complex(1.0, 1.0), 1, 1));
        mat_complex_B = new SparseMatrix<ArraySequence, Complex>(seq_comp_B, 2, 2);
    }

    void TearDown() override{
        delete empty_mat;
        delete zero_mat;
        delete mat_A;
        delete mat_B;
        delete mat_C;
        delete mat_complex_A;
        delete mat_complex_B;
    }
};


class Dict_Fixture : public testing::Test{
protected:
    Dict<Pair<int, int>, double, ArraySequence>* dict;

    void SetUp() override{
        dict = new Dict<Pair<int, int>, double, ArraySequence>;
    }

    void TearDown() override{
        delete dict;
    }
};


template <typename T, template <typename> class Container>
inline void show_matrix(const SparseMatrix<Container, T>& matrix){
    Container<Matrix_elem<T>> items = matrix.Get_Elements(); 
    std::cout << "Вывод элементов матрицы по индексу:" << std::endl;

    for (size_t k = 0; k < items.GetLength(); ++k){
        Matrix_elem<T> temp = items.Get(k); 
        std::cout << "Элемент по индексам (" << temp.i << ", " << temp.j  << ") = " << temp.elem << std::endl;
    }
    
}