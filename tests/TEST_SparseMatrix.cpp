#include "Fixtures.hpp"
#include <tuple>
#include <cmath>
#include <string>

class SparseMatrixDefConstructorTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<int, int>> {};
                                       
TEST_P(SparseMatrixDefConstructorTest, def_constructor){
    auto [expected_rows, expected_cols] = GetParam();
    MAT_EQ(empty_mat->Get_rows(), expected_rows, *empty_mat);
    MAT_EQ(empty_mat->Get_cols(), expected_cols, *empty_mat);
    MAT_THROW(empty_mat->Get_elem(0, 0), IndexOutOfRangeException, *empty_mat);
}
INSTANTIATE_TEST_SUITE_P(DefConstructorParams, SparseMatrixDefConstructorTest, testing::Values(
    std::make_tuple(0, 0)
));


class SparseMatrixSizeConstructorTest : public Sparse_Matrix_Fixture, 
                                        public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                        
TEST_P(SparseMatrixSizeConstructorTest, size_constructor){
    auto [expected_rows, expected_cols, expected_val] = GetParam();
    MAT_EQ(zero_mat->Get_rows(), expected_rows, *zero_mat);
    MAT_EQ(zero_mat->Get_cols(), expected_cols, *zero_mat);
    MAT_EQ(zero_mat->Get_elem(1, 1), expected_val, *zero_mat);
}
INSTANTIATE_TEST_SUITE_P(SizeConstructorParams, SparseMatrixSizeConstructorTest, testing::Values(
    std::make_tuple(3, 3, 0)
));


class SparseMatrixGetElemTest : public Sparse_Matrix_Fixture, 
                                public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                
TEST_P(SparseMatrixGetElemTest, get_elem){
    auto [row, col, expected] = GetParam();
    MAT_EQ(mat_A->Get_elem(row, col), expected, *mat_A);
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
    MAT_EQ(mat_A->Get_elem(row, col), expected, *mat_A);
}
INSTANTIATE_TEST_SUITE_P(GetElemZeroParams, SparseMatrixGetElemZeroTest, testing::Values(
    std::make_tuple(0, 1, 0),
    std::make_tuple(1, 0, 0)
));


class SparseMatrixParenthesesTest : public Sparse_Matrix_Fixture, 
                                    public testing::WithParamInterface<std::tuple<int, int, int>> {};
                                    
TEST_P(SparseMatrixParenthesesTest, operator_parentheses){
    auto [row, col, expected] = GetParam();
    MAT_EQ((*mat_A)(row, col), expected, *mat_A);
}
INSTANTIATE_TEST_SUITE_P(ParenthesesParams, SparseMatrixParenthesesTest, testing::Values(
    std::make_tuple(2, 0, 4),
    std::make_tuple(1, 2, 0)
));


class SparseMatrixInvalidArgTest : public Sparse_Matrix_Fixture, 
                                   public testing::WithParamInterface<std::tuple<int, int, std::string>> {};
                                   
TEST_P(SparseMatrixInvalidArgTest, get_elem_invalid_argument){
    auto [row, col, msg] = GetParam();
    MAT_THROW(mat_A->Get_elem(row, col), IndexOutOfRangeException, *mat_A);
}
INSTANTIATE_TEST_SUITE_P(InvalidArgParams, SparseMatrixInvalidArgTest, testing::Values(
    std::make_tuple(3, 0, ""),
    std::make_tuple(0, 5, "")
));


class SparseMatrixAddTest : public Sparse_Matrix_Fixture, 
                            public testing::WithParamInterface<std::tuple<int, int, int>> {};
                            
TEST_P(SparseMatrixAddTest, operator_add){
    SparseMatrix<ArraySequence, int> sum = (*mat_A) + (*mat_B);
    auto [row, col, expected] = GetParam();
    
    MAT_OP_EQ(sum.Get_rows(), 3, *orig_mat_A, *orig_mat_B, sum);
    MAT_OP_EQ(sum.Get_cols(), 3, *orig_mat_A, *orig_mat_B, sum);
    MAT_OP_EQ(sum(row, col), expected, *orig_mat_A, *orig_mat_B, sum);
}
INSTANTIATE_TEST_SUITE_P(AddParams, SparseMatrixAddTest, testing::Values(
    std::make_tuple(0, 0, 1),
    std::make_tuple(0, 2, 5), //4
    std::make_tuple(1, 0, 1),
    std::make_tuple(1, 1, 5),
    std::make_tuple(2, 0, 4),
    std::make_tuple(2, 2, 0)
));


class SparseMatrixAddInvalidSizeTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<bool>> {};
                                       
TEST_P(SparseMatrixAddInvalidSizeTest, operator_add_invalid_size){
    MAT_OP_THROW((*mat_A) + (*mat_C), InvalidSizeException, *mat_A, *mat_C);
}
INSTANTIATE_TEST_SUITE_P(AddInvalidSizeParams, SparseMatrixAddInvalidSizeTest, testing::Values(
    std::make_tuple(true)
));


class SparseMatrixMultTest : public Sparse_Matrix_Fixture, 
                             public testing::WithParamInterface<std::tuple<int, int, int>> {};
                             
TEST_P(SparseMatrixMultTest, operator_mult){
    SparseMatrix<ArraySequence, int> mult = (*mat_A) * (*mat_C);
    auto [row, col, expected] = GetParam();
    
    MAT_OP_EQ(mult.Get_rows(), 3, *mat_A, *mat_C, mult);
    MAT_OP_EQ(mult.Get_cols(), 2, *mat_A, *mat_C, mult);
    MAT_OP_EQ(mult(row, col), expected, *mat_A, *mat_C, mult);
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
    MAT_OP_THROW((*mat_C) * (*mat_A), InvalidSizeException, *mat_C, *mat_A);
}
INSTANTIATE_TEST_SUITE_P(MultInvalidSizeParams, SparseMatrixMultInvalidSizeTest, testing::Values(
    std::make_tuple(true)
));


class SparseMatrixNormTest : public Sparse_Matrix_Fixture, 
                             public testing::WithParamInterface<std::tuple<int, int, int, int, double, double>> {};
                             
TEST_P(SparseMatrixNormTest, get_norm){
    auto [norm_A, norm_B, norm_C, norm_zero, norm_comp_A, norm_comp_B] = GetParam();
    
    MAT_EQ(mat_A->Get_Norm(), norm_A, *mat_A);
    MAT_EQ(mat_B->Get_Norm(), norm_B, *mat_B);
    MAT_EQ(mat_C->Get_Norm(), norm_C, *mat_C);
    MAT_EQ(zero_mat->Get_Norm(), norm_zero, *zero_mat);
    
    Complex c_norm_A = mat_complex_A->Get_Norm();
    MAT_DOUBLE_EQ(c_norm_A.re, norm_comp_A, *mat_complex_A);
    MAT_DOUBLE_EQ(c_norm_A.im, 0.0, *mat_complex_A);

    Complex c_norm_B = mat_complex_B->Get_Norm();
    MAT_DOUBLE_EQ(c_norm_B.re, norm_comp_B, *mat_complex_B);
    MAT_DOUBLE_EQ(c_norm_B.im, 0.0, *mat_complex_B);
}
INSTANTIATE_TEST_SUITE_P(NormParams, SparseMatrixNormTest, testing::Values(
    std::make_tuple(7, 5, 5, 0, std::sqrt(29.0), std::sqrt(45.0))
));


class SparseMatrixComplexGetElemTest : public Sparse_Matrix_Fixture, 
                                       public testing::WithParamInterface<std::tuple<int, int, double, double>> {};
                                       
TEST_P(SparseMatrixComplexGetElemTest, complex_get_elem){
    auto [row, col, expected_re, expected_im] = GetParam();
    Complex c1 = mat_complex_A->Get_elem(row, col);
    
    MAT_EQ(c1.re, expected_re, *mat_complex_A);
    MAT_EQ(c1.im, expected_im, *mat_complex_A);
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
    
    MAT_OP_EQ(c.re, expected_re, *mat_complex_A, *mat_complex_B, sum);
    MAT_OP_EQ(c.im, expected_im, *mat_complex_A, *mat_complex_B, sum);
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
    
    MAT_OP_EQ(c.re, expected_re, *mat_complex_A, *mat_complex_B, mult);
    MAT_OP_EQ(c.im, expected_im, *mat_complex_A, *mat_complex_B, mult);
}
INSTANTIATE_TEST_SUITE_P(ComplexMultParams, SparseMatrixComplexMultTest, testing::Values(
    std::make_tuple(0, 0, 0.0, 5.0),
    std::make_tuple(1, 1, 4.0, 2.0)
));