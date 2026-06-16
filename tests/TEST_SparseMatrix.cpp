#include "Fixtures.hpp"
#include <tuple>
#include <cmath>
#include <string>

class SparseMatrixDefConstructorTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<int, int>> {};
                                       
TEST_P(SparseMatrixDefConstructorTest, def_constructor){
    auto [expected_rows, expected_cols] = GetParam();
    EXPECT_EQ(empty_mat->Get_rows(), expected_rows) << "Ожидается " << expected_rows << " строк. По факту: " << empty_mat->Get_rows();
    EXPECT_EQ(empty_mat->Get_cols(), expected_cols) << "Ожидается " << expected_cols << " столбцов. По факту: " << empty_mat->Get_cols();
    EXPECT_THROW(empty_mat->Get_elem(0, 0), IndexOutOfRangeException) << "Ожидается IndexOutOfRangeException при обращении к пустой матрице";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица (пустая)---\n";
        show_matrix(*empty_mat);
    }
}
INSTANTIATE_TEST_SUITE_P(DefConstructorParams, SparseMatrixDefConstructorTest, testing::Values(
    std::make_tuple(0, 0)
));

class SparseMatrixSizeConstructorTest : public Sparse_Matrix_Fixture, 
                                        public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                        
TEST_P(SparseMatrixSizeConstructorTest, size_constructor){
    auto [expected_rows, expected_cols, expected_val] = GetParam();
    EXPECT_EQ(zero_mat->Get_rows(), expected_rows) << "Ожидается " << expected_rows << " строки. По факту: " << zero_mat->Get_rows();
    EXPECT_EQ(zero_mat->Get_cols(), expected_cols) << "Ожидается " << expected_cols << " столбца. По факту: " << zero_mat->Get_cols();
    EXPECT_EQ(zero_mat->Get_elem(1, 1), expected_val) << "Ожидается " << expected_val << " для пустой ячейки. По факту: " << zero_mat->Get_elem(1, 1);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица (нулевая)---\n";
        show_matrix(*zero_mat);
    }
}
INSTANTIATE_TEST_SUITE_P(SizeConstructorParams, SparseMatrixSizeConstructorTest, testing::Values(
    std::make_tuple(3, 3, 0)
));




class SparseMatrixGetElemTest : public Sparse_Matrix_Fixture, 
                                public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                
TEST_P(SparseMatrixGetElemTest, get_elem){
    auto [row, col, expected] = GetParam();
    EXPECT_EQ(mat_A->Get_elem(row, col), expected) << "Ожидаемый элемент (" << row << "," << col << ") -> " << expected << ". По факту: " << mat_A->Get_elem(row, col);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица A---\n";
        show_matrix(*mat_A);
    }
}
INSTANTIATE_TEST_SUITE_P(GetElemParams, SparseMatrixGetElemTest, testing::Values(
    std::make_tuple(0, 0, 1),
    std::make_tuple(1, 1, 3),
    std::make_tuple(2, 2, 5)
));


class SparseMatrixGetElemZeroTest : public Sparse_Matrix_Fixture, 
                                    public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                    
TEST_P(SparseMatrixGetElemZeroTest, get_elem_zero){
    auto [row, col, expected] = GetParam();
    EXPECT_EQ(mat_A->Get_elem(row, col), expected) <<"Ожидаемый элемент (" << row << "," << col << ") -> " << expected << ". По факту: " << mat_A->Get_elem(row, col);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица A---\n";
        show_matrix(*mat_A);
    }
}
INSTANTIATE_TEST_SUITE_P(GetElemZeroParams, SparseMatrixGetElemZeroTest, testing::Values(
    std::make_tuple(0, 1, 0),
    std::make_tuple(1, 0, 0)
));




class SparseMatrixParenthesesTest : public Sparse_Matrix_Fixture, 
                                    public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                    
TEST_P(SparseMatrixParenthesesTest, operator_parentheses){
    auto [row, col, expected] = GetParam();
    EXPECT_EQ((*mat_A)(row, col), expected) << "Ожидаемый элемент (" << row << "," << col << ") через оператор () -> " << expected << ". По факту: " << (*mat_A)(row, col);
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица A---\n";
        show_matrix(*mat_A);
    }
}
INSTANTIATE_TEST_SUITE_P(ParenthesesParams, SparseMatrixParenthesesTest, testing::Values(
    std::make_tuple(2, 0, 4),
    std::make_tuple(1, 2, 0)
));




class SparseMatrixInvalidArgTest : public Sparse_Matrix_Fixture, 
                                   public testing::WithParamInterface<std::tuple<int, int, std::string>> {};
                                   
TEST_P(SparseMatrixInvalidArgTest, get_elem_invalid_argument){
    auto [row, col, msg] = GetParam();
    EXPECT_THROW(mat_A->Get_elem(row, col), IndexOutOfRangeException) << msg;
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая матрица A---\n";
        show_matrix(*mat_A);
    }
}
INSTANTIATE_TEST_SUITE_P(InvalidArgParams, SparseMatrixInvalidArgTest, testing::Values(
    std::make_tuple(3, 0, "Ожидается IndexOutOfRangeException для строки вне диапазона"),
    std::make_tuple(0, 5, "Ожидается IndexOutOfRangeException для столбца вне диапазона")
));



class SparseMatrixAddTest : public Sparse_Matrix_Fixture, 
                            public testing::WithParamInterface<std::tuple<int, int, int>> {};
                            
TEST_P(SparseMatrixAddTest, operator_add){
    SparseMatrix<ArraySequence, int> sum = (*mat_A) + (*mat_B);
    auto [row, col, expected] = GetParam();
    
    EXPECT_EQ(sum.Get_rows(), 3) << "Ожидается 3 строки.";
    EXPECT_EQ(sum.Get_cols(), 3) << "Ожидается 3 столбца.";
    
    EXPECT_EQ(sum(row, col), expected) << "Ожидаемый элемент по инжексу (" << row << ", " << col << ") : " << expected;
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор + для матриц A и B---\n";
        std::cout << "\n---Матрица A---\n";
        show_matrix(*orig_mat_A);
        std::cout << "\n---Матрица B---\n";
        show_matrix(*orig_mat_B);
        std::cout << "\n---Текущая матрица (сумма)---\n";
        show_matrix(sum);
    }
}



INSTANTIATE_TEST_SUITE_P(AddParams, SparseMatrixAddTest, testing::Values(
    std::make_tuple(0, 0, 1),
    std::make_tuple(0, 2, 4),
    std::make_tuple(1, 0, 1),
    std::make_tuple(1, 1, 5),
    std::make_tuple(2, 0, 4),
    std::make_tuple(2, 2, 0)
));



class SparseMatrixAddInvalidSizeTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<bool>> {};
                                       
TEST_P(SparseMatrixAddInvalidSizeTest, operator_add_invalid_size){
    EXPECT_THROW((*mat_A) + (*mat_C), InvalidSizeException) << "Ожидается InvalidSizeException при сложении матриц (3x3 и 3x2)";

    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор + для матриц A и C (несовпадение размеров)---\n";
        std::cout << "\n---Матрица A---\n";
        show_matrix(*mat_A);
        std::cout << "\n---Матрица C---\n";
        show_matrix(*mat_C);
    }
}
INSTANTIATE_TEST_SUITE_P(AddInvalidSizeParams, SparseMatrixAddInvalidSizeTest, testing::Values(
    std::make_tuple(true)
));

class SparseMatrixMultTest : public Sparse_Matrix_Fixture, 
                             public testing::WithParamInterface<std::tuple<int, int, int>> {};
                             
TEST_P(SparseMatrixMultTest, operator_mult){
    SparseMatrix<ArraySequence, int> mult = (*mat_A) * (*mat_C);
    auto [row, col, expected] = GetParam();
    
    EXPECT_EQ(mult.Get_rows(), 3) << "Результирующая матрица должна иметь 3 строки";
    EXPECT_EQ(mult.Get_cols(), 2) << "Результирующая матрица должна иметь 2 столбца";

    EXPECT_EQ(mult(row, col), expected) << "Ожидаемый результат для ячейки (" << row << "," << col << ") " << expected;
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор * для матриц A и C---\n";
        std::cout << "\n---Матрица A---\n";
        show_matrix(*mat_A);
        std::cout << "\n---Матрица C---\n";
        show_matrix(*mat_C);
        std::cout << "\n---Текущая матрица (результат умножения)---\n";
        show_matrix(mult);
    }
}
INSTANTIATE_TEST_SUITE_P(MultParams, SparseMatrixMultTest, testing::Values(
    std::make_tuple(0, 0, 9),
    std::make_tuple(0, 1, 2),
    std::make_tuple(1, 0, 0),
    std::make_tuple(1, 1, 9),
    std::make_tuple(2, 0, 24),
    std::make_tuple(2, 1, 8)
));

class SparseMatrixMultInvalidSizeTest : public Sparse_Matrix_Fixture, 
                                        public testing::WithParamInterface<std::tuple<bool>> {};
                                       
TEST_P(SparseMatrixMultInvalidSizeTest, operator_mult_invalid_size){
    EXPECT_THROW((*mat_C) * (*mat_A), InvalidSizeException) << "Ожидается InvalidSizeException при умножении несовместимых матриц";

    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор * для матриц C и A (несовпадение размеров)---\n";
        std::cout << "\n---Матрица C---\n";
        show_matrix(*mat_C);
        std::cout << "\n---Матрица A---\n";
        show_matrix(*mat_A);
    }
}
INSTANTIATE_TEST_SUITE_P(MultInvalidSizeParams, SparseMatrixMultInvalidSizeTest, testing::Values(
    std::make_tuple(true)
));

class SparseMatrixNormTest : public Sparse_Matrix_Fixture, 
                             public testing::WithParamInterface<std::tuple<int, int, int, int, double, double>> {};
                             
TEST_P(SparseMatrixNormTest, get_norm){
    auto [norm_A, norm_B, norm_C, norm_zero, norm_comp_A, norm_comp_B] = GetParam();
    
    EXPECT_EQ(mat_A->Get_Norm(), norm_A) << "Ожидается норма " << norm_A << " для mat_A. По факту: " << mat_A->Get_Norm();
    EXPECT_EQ(mat_B->Get_Norm(), norm_B) << "Ожидается норма " << norm_B << " для mat_B (sqrt(34) -> 5). По факту: " << mat_B->Get_Norm();
    EXPECT_EQ(mat_C->Get_Norm(), norm_C) << "Ожидается норма " << norm_C << " для mat_C (sqrt(30) -> 5). По факту: " << mat_C->Get_Norm();
    EXPECT_EQ(zero_mat->Get_Norm(), norm_zero) << "Норма нулевой матрицы должна быть строго " << norm_zero;
    
    Complex c_norm_A = mat_complex_A->Get_Norm();
    EXPECT_DOUBLE_EQ(c_norm_A.re, norm_comp_A) << "Ожидаемая норма для mat_complex_A -> sqrt(" << (norm_comp_A * norm_comp_A) << ")";
    EXPECT_DOUBLE_EQ(c_norm_A.im, 0.0) << "Мнимая часть нормы должна быть строго 0";

    Complex c_norm_B = mat_complex_B->Get_Norm();
    EXPECT_DOUBLE_EQ(c_norm_B.re, norm_comp_B) << "Ожидаемая норма для mat_complex_B -> sqrt(" << (norm_comp_B * norm_comp_B) << ")";
    EXPECT_DOUBLE_EQ(c_norm_B.im, 0.0) << "Мнимая часть нормы должна быть строго 0";
    
    if (testing::Test::HasFailure()){
        std::cout << "\n---Проверка норм матриц---\n";
        std::cout << "\n---Матрица A---\n";
        show_matrix(*mat_A);
        std::cout << "\n---Комплексная матрица A---\n";
        show_matrix(*mat_complex_A);
    }
}
INSTANTIATE_TEST_SUITE_P(NormParams, SparseMatrixNormTest, testing::Values(
    std::make_tuple(7, 5, 5, 0, std::sqrt(29.0), std::sqrt(45.0))
));

class SparseMatrixComplexGetElemTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<int, int, double, double>> {};
                                       
TEST_P(SparseMatrixComplexGetElemTest, complex_get_elem){
    auto [row, col, expected_re, expected_im] = GetParam();
    Complex c1 = mat_complex_A->Get_elem(row, col);
    
    EXPECT_EQ(c1.re, expected_re) << "Ожидаемая действительная часть для (" << row << "," << col << ") -> " << expected_re;
    EXPECT_EQ(c1.im, expected_im) << "Ожидаемая мнимая часть для (" << row << "," << col << ") -> " << expected_im;

    if (testing::Test::HasFailure()){
        std::cout << "\n---Текущая комплексная матрица A---\n";
        show_matrix(*mat_complex_A);
    }
}
INSTANTIATE_TEST_SUITE_P(ComplexGetElemParams, SparseMatrixComplexGetElemTest, testing::Values(
    std::make_tuple(0, 0, 1.0, 2.0),
    std::make_tuple(0, 1, 0.0, 0.0)
));

class SparseMatrixComplexSumTest : public Sparse_Matrix_Fixture, 
                                   public testing::WithParamInterface<std::tuple<int, int, double, double>> {};
                                   
TEST_P(SparseMatrixComplexSumTest, complex_operator_sum){
    SparseMatrix<ArraySequence, Complex> sum = (*mat_complex_A) + (*mat_complex_B);
    auto [row, col, expected_re, expected_im] = GetParam();
    Complex c = sum(row, col); 
    
    EXPECT_EQ(c.re, expected_re) << "Сложение -> действительная часть " << expected_re;
    EXPECT_EQ(c.im, expected_im) << "Сложение -> мнимая часть " << expected_im;

    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор + для комплексных матриц A и B---\n";
        std::cout << "\n---Комплексная матрица A---\n";
        show_matrix(*mat_complex_A);
        std::cout << "\n---Комплексная матрица B---\n";
        show_matrix(*mat_complex_B);
        std::cout << "\n---Текущая матрица (сумма)---\n";
        show_matrix(sum);
    }
}
INSTANTIATE_TEST_SUITE_P(ComplexSumParams, SparseMatrixComplexSumTest, testing::Values(
    std::make_tuple(0, 0, 3.0, 3.0),
    std::make_tuple(1, 1, 4.0, 0.0)
));

class SparseMatrixComplexMultTest : public Sparse_Matrix_Fixture, 
                                    public testing::WithParamInterface<std::tuple<int, int, double, double>> {};
                                    
TEST_P(SparseMatrixComplexMultTest, complex_operator_mult){
    SparseMatrix<ArraySequence, Complex> mult = (*mat_complex_A) * (*mat_complex_B);
    auto [row, col, expected_re, expected_im] = GetParam();
    Complex c = mult(row, col);
    
    EXPECT_EQ(c.re, expected_re) << "Умножение -> действительная часть " << expected_re;
    EXPECT_EQ(c.im, expected_im) << "Умножение -> мнимая часть " << expected_im;

    if (testing::Test::HasFailure()){
        std::cout << "\n---Тестируем оператор * для комплексных матриц A и B---\n";
        std::cout << "\n---Комплексная матрица A---\n";
        show_matrix(*mat_complex_A);
        std::cout << "\n---Комплексная матрица B---\n";
        show_matrix(*mat_complex_B);
        std::cout << "\n---Текущая матрица (результат умножения)---\n";
        show_matrix(mult);
    }
}
INSTANTIATE_TEST_SUITE_P(ComplexMultParams, SparseMatrixComplexMultTest, testing::Values(
    std::make_tuple(0, 0, 0.0, 5.0),
    std::make_tuple(1, 1, 4.0, 2.0)
));