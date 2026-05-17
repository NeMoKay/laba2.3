#include "Fixtures.hpp"


TEST_F(Sparse_Matrix_Fixture, def_constructor) {
    EXPECT_EQ(empty_mat->Get_rows(), 0) << "Ожидается 0 строк. По факту: " << empty_mat->Get_rows();
    EXPECT_EQ(empty_mat->Get_cols(), 0) << "Ожидается 0 столбцов. По факту: " << empty_mat->Get_cols();
    EXPECT_THROW(empty_mat->Get_elem(0, 0), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException при обращении к пустой матрице";
}

TEST_F(Sparse_Matrix_Fixture, size_constructor) {
    EXPECT_EQ(zero_mat->Get_rows(), 3) << "Ожидается 3 строки. По факту: " << zero_mat->Get_rows();
    EXPECT_EQ(zero_mat->Get_cols(), 3) << "Ожидается 3 столбца. По факту: " << zero_mat->Get_cols();
    EXPECT_EQ(zero_mat->Get_elem(1, 1), 0) << "Ожидается 0 для пустой ячейки. По факту: " << zero_mat->Get_elem(1, 1);
}


TEST_F(Sparse_Matrix_Fixture, get_elem_valid) {
    EXPECT_EQ(mat_A->Get_elem(0, 0), 1) << "Ожидаемый элемент (0,0) -> 1. По факту: " << mat_A->Get_elem(0, 0);
    EXPECT_EQ(mat_A->Get_elem(1, 1), 3) << "Ожидаемый элемент (1,1) -> 3. По факту: " << mat_A->Get_elem(1, 1);
    EXPECT_EQ(mat_A->Get_elem(2, 2), 5) << "Ожидаемый элемент (2,2) -> 5. По факту: " << mat_A->Get_elem(2, 2);
}

TEST_F(Sparse_Matrix_Fixture, get_elem_zeroes) {
    EXPECT_EQ(mat_A->Get_elem(0, 1), 0) << "Ожидаемый элемент (0,1) -> 0. По факту: " << mat_A->Get_elem(0, 1);
    EXPECT_EQ(mat_A->Get_elem(1, 0), 0) << "Ожидаемый элемент (1,0) -> 0. По факту: " << mat_A->Get_elem(1, 0);
}

TEST_F(Sparse_Matrix_Fixture, operator_parens) {
    EXPECT_EQ((*mat_A)(2, 0), 4) << "Ожидаемый элемент (2,0) через оператор () -> 4. По факту: " << (*mat_A)(2, 0);
    EXPECT_EQ((*mat_A)(1, 2), 0) << "Ожидаемый элемент (1,2) через оператор () -> 0. По факту: " << (*mat_A)(1, 2);
}

TEST_F(Sparse_Matrix_Fixture, get_elem_invalid_argument) {
    EXPECT_THROW(mat_A->Get_elem(3, 0), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException для строки вне диапазона";
    EXPECT_THROW(mat_A->Get_elem(0, 5), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException для столбца вне диапазона";
}


TEST_F(Sparse_Matrix_Fixture, operator_add) {
    SparseMatrix<ArraySequence, int> sum = (*mat_A) + (*mat_B);
    
    EXPECT_EQ(sum.Get_rows(), 3) << "Ожидается 3 строки.";
    EXPECT_EQ(sum.Get_cols(), 3) << "Ожидается 3 столбца.";
    
    EXPECT_EQ(sum(0, 0), 1) << "1 + 0 = 1";
    EXPECT_EQ(sum(0, 2), 4) << "2 + 2 = 4";
    EXPECT_EQ(sum(1, 0), 1) << "0 + 1 = 1";
    EXPECT_EQ(sum(1, 1), 5) << "3 + 2 = 5";
    EXPECT_EQ(sum(2, 0), 4) << "4 + 0 = 4";
    EXPECT_EQ(sum(2, 2), 0) << "5 + (-5) = 0 (взаимоуничтожились)";
}

TEST_F(Sparse_Matrix_Fixture, operator_add_invalid_size) {
    EXPECT_THROW((*mat_A) + (*mat_C), InvalidSizeException) << "Ожидается InvalidSizeException при сложении матриц (3x3 и 3x2)";
}

TEST_F(Sparse_Matrix_Fixture, operator_mult) {
    // mat_A (3x3) * mat_C (3x2)
    SparseMatrix<ArraySequence, int> mult = (*mat_A) * (*mat_C);
    
    EXPECT_EQ(mult.Get_rows(), 3) << "Результирующая матрица должна иметь 3 строки";
    EXPECT_EQ(mult.Get_cols(), 2) << "Результирующая матрица должна иметь 2 столбца";

    EXPECT_EQ(mult(0, 0), 9);

    EXPECT_EQ(mult(0, 1), 2);

    EXPECT_EQ(mult(1, 0), 0);

    EXPECT_EQ(mult(1, 1), 9);

    EXPECT_EQ(mult(2, 0), 24);

    EXPECT_EQ(mult(2, 1), 8);
}

TEST_F(Sparse_Matrix_Fixture, operator_mult_invalid_size) {

    EXPECT_THROW((*mat_C) * (*mat_A), InvalidSizeException) << "Ожидается InvalidSizeException при умножении несовместимых матриц";
}

// --- Норма ---

TEST_F(Sparse_Matrix_Fixture, get_norm) {
    EXPECT_EQ(mat_A->Get_Norm(), 7) << "Ожидается норма 7 (каст корня из 55 к int). По факту: " << mat_A->Get_Norm();
    EXPECT_EQ(zero_mat->Get_Norm(), 0) << "Норма нулевой матрицы должна быть 0";
}


