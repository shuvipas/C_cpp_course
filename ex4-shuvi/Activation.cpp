#include "Activation.h"
#include <cmath>


Matrix activation::relu(const Matrix &matrix)
{
    Matrix n_mat(matrix.get_rows(), matrix.get_cols());
    for (int r = 0; r < matrix.get_rows(); ++r)
    {
        for (int c = 0; c < matrix.get_cols(); ++c)
        {
            if (matrix(r,c) < 0)
            {
                n_mat(r,c) = 0;
            }
            else
            {
                n_mat(r,c) = matrix(r,c);
            }
        }
    }
    return n_mat;
}

Matrix activation::softmax(const Matrix &matrix)
{
    Matrix n_mat(matrix.get_rows(), matrix.get_cols());
    float exp_sum =0.0;
    for (int r = 0; r < matrix.get_rows(); ++r)
    {
        for (int c = 0; c < matrix.get_cols(); ++c)
        {

            n_mat(r,c) = std::exp(matrix(r,c));
            exp_sum +=n_mat(r,c);
        }
    }
    
    for (int r = 0; r < matrix.get_rows(); r++)
    {
        for (int c = 0; c < matrix.get_cols(); c++)
        {
            n_mat(r,c) *= 1/exp_sum;
        }
    }
    return n_mat;
}
