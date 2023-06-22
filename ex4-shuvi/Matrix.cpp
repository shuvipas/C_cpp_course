#include "Matrix.h"
#include <cmath>

#define DIMEN_ERR "Invalid matrix dimensions"
#define RANGE_ERR "Invalid input (out of range)"
#define IN_ERR "Input matrix failed"

#define SPACE " "
#define BIT_PRINT_1 "**"
#define BIT_PRINT_0 "  "
#define MIN_1_SIZE 0.1

Matrix::Matrix(int r, int c)
{
    if (r <= 0 || c <= 0)
    {
        throw std::length_error(DIMEN_ERR);
    }
    rows = r;
    cols = c;
    data = new float *[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new float[cols];
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = 0;
        }
    }
}

void Matrix::copy_matrix(const Matrix &matrix)
{
    rows = matrix.rows;
    cols = matrix.cols;
    data = new float *[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new float[cols];
        for (int j = 0; j < cols; j++)
        {
            data[i][j] = matrix.data[i][j];
        }
    }
}
Matrix::Matrix(const Matrix &matrix)
{
    Matrix::copy_matrix(matrix);
}

Matrix::~Matrix()
{
    for (int r = 0; r < rows; r++)
    {
        delete[] data[r];
    }
    delete[] data;
}

Matrix &Matrix::transpose()
{
    Matrix n_mat(cols, rows);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            n_mat.data[c][r] = data[r][c];
        }
    }
    *this = n_mat;
    return *this;
}

Matrix &Matrix::vectorize()
{
    Matrix n_mat(rows * cols, 1);
    int index = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            n_mat.data[index][0] = data[r][c];
            ++index;
        }
    }
    *this = n_mat;
    return *this;
}
void Matrix::plain_print() const
{
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            std::cout << data[r][c] << SPACE;
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::dot(const Matrix &matrix) const
{
    if (rows != matrix.rows || cols != matrix.cols)
    {
        throw std::length_error(DIMEN_ERR);
    }
    Matrix n_mat(rows, cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            n_mat.data[r][c] = data[r][c] * matrix.data[r][c];
        }
    }
    return n_mat;
}

float Matrix::sum() const
{
    float sum = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            sum += data[r][c];
        }
    }
    return sum;
}

float Matrix::norm() const
{
    float normal = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            normal += data[r][c] * data[r][c];
        }
    }
    return std::sqrt(normal);
}

int Matrix::argmax() const
{
    float max = data[0][0];
    int index = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (data[r][c] > max)
            {
                max = data[r][c];
                index = r * cols + c;
            }
        }
    }
    return index;
}

Matrix Matrix::operator+(const Matrix &matrix) const
{
    if (rows != matrix.rows || cols != matrix.cols)
    {
        throw std::length_error(DIMEN_ERR);
    }
    Matrix n_mat(rows, cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            n_mat.data[r][c] = data[r][c] + matrix.data[r][c];
        }
    }
    return n_mat;
}

Matrix &Matrix::operator+=(const Matrix &matrix)
{
    if (rows != matrix.rows || cols != matrix.cols)
    {
        throw std::length_error(DIMEN_ERR);
    }
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            data[r][c] += matrix.data[r][c];
        }
    }
    return *this;
}


Matrix operator*(const Matrix &lhs, const Matrix &rhs) 
{
    if (lhs.cols != rhs.rows)
    {
        throw std::length_error(DIMEN_ERR);
    }
    Matrix n_mat(lhs.rows, rhs.cols);
    for (int i = 0; i < lhs.rows; ++i)
    {
        for (int j = 0; j < rhs.cols; ++j)
        {
            for (int k = 0; k < lhs.cols; ++k)
            {
                n_mat.data[i][j] += lhs.data[i][k] * rhs.data[k][j];
            }
        }
    }
    return n_mat;
}

Matrix Matrix::operator*(const float &f) const
{
    Matrix n_mat(rows, cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            n_mat.data[r][c] = data[r][c] * f;
        }
    }
    return n_mat;
}

Matrix operator*(const float &f, const Matrix &matrix)
{
    return matrix * f;
}

Matrix &Matrix::operator=(const Matrix &matrix)
{
    if (this != &matrix)
    {
        this->~Matrix();
        Matrix::copy_matrix(matrix);
    }
    return *this;
}
float& Matrix::operator()(int i, int j)
{
    if (i >= rows || j >= cols)
    {
        throw std::length_error(RANGE_ERR);
    }
    float &val = data[i][j];
    return val;
}
const float &Matrix::operator()(int i, int j) const
{
    if (i >= rows || j >= cols)
    {
        throw std::length_error(RANGE_ERR);
    }
    const float &val = data[i][j];
    return val;
}

float &Matrix::operator[](int i) 
{
    if (i >= rows * cols)
    {
        throw std::length_error(RANGE_ERR);
    }
    float &val = data[i / cols][i % cols];
    
    return val;
}

const float &Matrix::operator[](int i) const
{
    if (i >= rows * cols)
    {
        throw std::length_error(RANGE_ERR);
    }
    const float &val = data[i / cols][i % cols];
    ;
    return val;
}
std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    for (int r = 0; r < matrix.rows; r++)
    {
        for (int c = 0; c < matrix.cols; c++)
        {
            if (matrix.data[r][c] > MIN_1_SIZE)
            {
                os << BIT_PRINT_1;
            }
            else
            {
                os << BIT_PRINT_0;
            }
        }
        os << std::endl;
    }
    return os;
}

std::istream &operator>>(std::istream &is, Matrix &matrix)
{
    for (int r = 0; r < matrix.rows; ++r)
    {
        for (int c = 0; c < matrix.cols; ++c)
        {
            if (!is.good())
            {
                throw std::runtime_error(IN_ERR);
            }
            is.read((char *) &matrix(r, c), sizeof(float));
        }
    }
    return is;
}
