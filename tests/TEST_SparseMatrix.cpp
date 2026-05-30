#include "Fixtures.hpp"

TEST_F(Sparse_Matrix_Fixture, def_constructor){
    EXPECT_EQ(empty_mat->Get_rows(), 0) << "Ожидается 0 строк. По факту: " << empty_mat->Get_rows();
    EXPECT_EQ(empty_mat->Get_cols(), 0) << "Ожидается 0 столбцов. По факту: " << empty_mat->Get_cols();
    EXPECT_THROW(empty_mat->Get_elem(0, 0), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException при обращении к пустой матрице";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
    }
}

TEST_F(Sparse_Matrix_Fixture, size_constructor){
    EXPECT_EQ(zero_mat->Get_rows(), 3) << "Ожидается 3 строки. По факту: " << zero_mat->Get_rows();
    EXPECT_EQ(zero_mat->Get_cols(), 3) << "Ожидается 3 столбца. По факту: " << zero_mat->Get_cols();
    EXPECT_EQ(zero_mat->Get_elem(1, 1), 0) << "Ожидается 0 для пустой ячейки. По факту: " << zero_mat->Get_elem(1, 1);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*zero_mat); 
    }
}

TEST_F(Sparse_Matrix_Fixture, get_elem){
    EXPECT_EQ(mat_A->Get_elem(0, 0), 1) << "Ожидаемый элемент (0,0) -> 1. По факту: " << mat_A->Get_elem(0, 0);
    EXPECT_EQ(mat_A->Get_elem(1, 1), 3) << "Ожидаемый элемент (1,1) -> 3. По факту: " << mat_A->Get_elem(1, 1);
    EXPECT_EQ(mat_A->Get_elem(2, 2), 5) << "Ожидаемый элемент (2,2) -> 5. По факту: " << mat_A->Get_elem(2, 2);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, get_elem_zero){
    EXPECT_EQ(mat_A->Get_elem(0, 1), 0) <<"Ожидаемый элемент (0,1) -> 0. По факту: " << mat_A->Get_elem(0, 1);
    EXPECT_EQ(mat_A->Get_elem(1, 0), 0) << "Ожидаемый элемент (1,0) -> 0. По факту: " << mat_A->Get_elem(1, 0);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, operator_parentheses){
    EXPECT_EQ((*mat_A)(2, 0), 4) << "Ожидаемый элемент (2,0) через оператор () -> 4. По факту: " << (*mat_A)(2, 0);
    EXPECT_EQ((*mat_A)(1, 2), 0) << "Ожидаемый элемент (1,2) через оператор () -> 0. По факту: " << (*mat_A)(1, 2);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, get_elem_invalid_argument){
    EXPECT_THROW(mat_A->Get_elem(3, 0), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException для строки вне диапазона";
    EXPECT_THROW(mat_A->Get_elem(0, 5), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException для столбца вне диапазона";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, operator_add){
    SparseMatrix<ArraySequence, int> sum = (*mat_A) + (*mat_B);
    
    EXPECT_EQ(sum.Get_rows(), 3) << "Ожидается 3 строки.";
    EXPECT_EQ(sum.Get_cols(), 3) << "Ожидается 3 столбца.";
    
    EXPECT_EQ(sum(0, 0), 1) << "Ожидаемый элемент по инжексу (0, 0) : 1";
    EXPECT_EQ(sum(0, 2), 4) << "Ожидаемый элемент по инжексу (0, 2) : 4";
    EXPECT_EQ(sum(1, 0), 1) << "Ожидаемый элемент по инжексу (1, 0) : 1";
    EXPECT_EQ(sum(1, 1), 5) << "Ожидаемый элемент по инжексу (1, 1) : 5";
    EXPECT_EQ(sum(2, 0), 4) << "Ожидаемый элемент по инжексу (2, 0) : 4";
    EXPECT_EQ(sum(2, 2), 0) << "Ожидаемый элемент по инжексу (2, 2) : 0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор + для матриц A и B---\n";
        std::cout << "\n---Матрица A---\n";
        show_matrix(*orig_mat_A);
        std::cout << "\n---Матрица B---\n";
        show_matrix(*orig_mat_B);
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(sum);
    }
}

TEST_F(Sparse_Matrix_Fixture, operator_add_invalid_size){
    EXPECT_THROW((*mat_A) + (*mat_C), InvalidSizeException) << "Ожидается InvalidSizeException при сложении матриц (3x3 и 3x2)";
}

TEST_F(Sparse_Matrix_Fixture, operator_mult){
    SparseMatrix<ArraySequence, int> mult = (*mat_A) * (*mat_C);
    
    EXPECT_EQ(mult.Get_rows(), 3) << "Результирующая матрица должна иметь 3 строки";
    EXPECT_EQ(mult.Get_cols(), 2) << "Результирующая матрица должна иметь 2 столбца";

    EXPECT_EQ(mult(0, 0), 9) << "Ожидаемый результат для ячейки (0,0) 9";
    EXPECT_EQ(mult(0, 1), 2) << "Ожидаемый результат для ячейки (0,1) 2";
    EXPECT_EQ(mult(1, 0), 0) << "Ожидаемый результат для ячейки (1,0) 0";
    EXPECT_EQ(mult(1, 1), 9) << "Ожидаемый результат для ячейки (1,1) 9";
    EXPECT_EQ(mult(2, 0), 24) << "Ожидаемый результат для ячейки (2,0) 24";
    EXPECT_EQ(mult(2, 1), 8) << "Ожидаемый результат для ячейки (2,1) 8";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(mult);
    }
}

TEST_F(Sparse_Matrix_Fixture, operator_mult_invalid_size){
    EXPECT_THROW((*mat_C) * (*mat_A), InvalidSizeException) << "Ожидается InvalidSizeException при умножении несовместимых матриц";
}

TEST_F(Sparse_Matrix_Fixture, get_norm){
    EXPECT_EQ(mat_A->Get_Norm(), 7) << "Ожидается норма 7 для mat_A. По факту: " << mat_A->Get_Norm();
    EXPECT_EQ(mat_B->Get_Norm(), 5) << "Ожидается норма 5 для mat_B (sqrt(34) -> 5). По факту: " << mat_B->Get_Norm();
    EXPECT_EQ(mat_C->Get_Norm(), 5) << "Ожидается норма 5 для mat_C (sqrt(30) -> 5). По факту: " << mat_C->Get_Norm();
    EXPECT_EQ(zero_mat->Get_Norm(), 0) << "Норма нулевой матрицы должна быть строго 0";
    
    Complex norm_A = mat_complex_A->Get_Norm();
    EXPECT_DOUBLE_EQ(norm_A.re, std::sqrt(29.0)) << "Ожидаемая норма для mat_complex_A -> sqrt(29)";
    EXPECT_DOUBLE_EQ(norm_A.im, 0.0) << "Мнимая часть нормы должна быть строго 0";

    Complex norm_B = mat_complex_B->Get_Norm();
    EXPECT_DOUBLE_EQ(norm_B.re, std::sqrt(45.0)) << "Ожидаемая норма для mat_complex_B -> sqrt(45)";
    EXPECT_DOUBLE_EQ(norm_B.im, 0.0) << "Мнимая часть нормы должна быть строго 0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_complex_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, complex_get_elem){
    Complex c1 = mat_complex_A->Get_elem(0, 0);
    EXPECT_EQ(c1.re, 1.0) << "Ожидаемая действительная часть для (0,0) -> 1.0";
    EXPECT_EQ(c1.im, 2.0) << "Ожидаемая мнимая часть для (0,0) -> 2.0";

    Complex c_zero = mat_complex_A->Get_elem(0, 1);
    EXPECT_EQ(c_zero.re, 0.0) << "Ожидаемая действительная часть пустой ячейки -> 0.0";
    EXPECT_EQ(c_zero.im, 0.0) << "Ожидаемая мнимая часть пустой ячейки -> 0.0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(*mat_complex_A); 
    }
}

TEST_F(Sparse_Matrix_Fixture, complex_operator_sum){
    SparseMatrix<ArraySequence, Complex> sum = (*mat_complex_A) + (*mat_complex_B);

    Complex c1 = sum(0, 0); 
    EXPECT_EQ(c1.re, 3.0) << "Сложение (1+2i) + (2+1i) -> действительная часть 3.0";
    EXPECT_EQ(c1.im, 3.0) << "Сложение (1+2i) + (2+1i) -> мнимая часть 3.0";

    Complex c2 = sum(1, 1);
    EXPECT_EQ(c2.re, 4.0) << "Сложение (3-1i) + (1+1i) -> действительная часть 4.0";
    EXPECT_EQ(c2.im, 0.0) << "Сложение (3-1i) + (1+1i) -> мнимая часть 0.0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(sum); 
    }
}

TEST_F(Sparse_Matrix_Fixture, complex_operator_mult){
    SparseMatrix<ArraySequence, Complex> mult = (*mat_complex_A) * (*mat_complex_B);

    Complex c1 = mult(0, 0);
    EXPECT_EQ(c1.re, 0.0) << "Умножение (1+2i) * (2+1i) -> действительная часть 0.0";
    EXPECT_EQ(c1.im, 5.0) << "Умножение (1+2i) * (2+1i) -> мнимая часть 5.0";

    Complex c2 = mult(1, 1);
    EXPECT_EQ(c2.re, 4.0) << "Умножение (3-1i) * (1+1i) -> действительная часть 4.0";
    EXPECT_EQ(c2.im, 2.0) << "Умножение (3-1i) * (1+1i) -> мнимая часть 2.0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица---\n";
        show_matrix(mult); 
    }
}