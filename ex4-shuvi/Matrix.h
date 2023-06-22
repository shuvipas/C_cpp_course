// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>


#define INIT_SIZE 1


/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

/**
 * @class Matrix
 * Represents a matrix of floating-point numbers.
 */
class Matrix
{
private:
	float **data;
	int rows;
	int cols;
	void copy_matrix(const Matrix &matrix);

public:
	/**
	 * Constructor for Matrix class.
	 * @param r The number of rows in the matrix.
	 * @param c The number of columns in the matrix.
	 */
	Matrix(int r, int c);
	Matrix() : Matrix(INIT_SIZE, INIT_SIZE) {}
	Matrix(const Matrix &matrix);
	~Matrix();

	int get_rows() const { return rows; }
	int get_cols() const { return cols; }

	
	/**
	 * @brief Transposes the matrix in-place.
	 * @return Reference to the transposed matrix.
	 */
	Matrix &transpose();
	/**
	 * @brief Converts the matrix into a column vector in-place.
	 * @return Reference to the vectorized matrix.
	 */
	Matrix &vectorize();
	/**
	 * @brief Prints the matrix in a plain format.
	 */
	void plain_print() const;
	/**
	 * @brief Computes the element-wise dot product of this matrix
	 *  and another matrix.
	 * @param matrix The other matrix for the dot product.
	 * @return The result of the dot product as a new matrix.
	 */
	Matrix dot(const Matrix &matrix) const;
	/**
	 * @brief Computes the sum of all elements in the matrix.
	 * @return The sum of all elements.
	 */
	float sum() const;

	/**
	 * @brief Computes the Euclidean norm of the matrix.
	 * @return The Euclidean norm.
	 */
	float norm() const;
	/**
	 * @brief Returns the index of the maximum element in the matrix.
	 * @return The index of the maximum element.
	 */
	int argmax() const;


	/**
	 * @brief Adds another matrix to this matrix and returns the result
	 *  as a new matrix.
	 * @param matrix The matrix to be added.
	 * @return The result of the addition as a new matrix.
	 */
	Matrix operator+(const Matrix &matrix) const;

	/**
	 * @brief Adds another matrix to this matrix in-place.
	 * @param matrix The matrix to be added.
	 * @return Reference to the updated matrix.
	 */
	Matrix &operator+=(const Matrix &matrix);

	friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
	/**
	 * @brief Multiplies this matrix by a scalar value and returns
	 *  the result as a new matrix.
	 * @param f The scalar value to multiply by.
	 * @return The result of the multiplication as a new matrix.
	 */
	Matrix operator*(const float &f) const;
	/**
	 * @brief Multiplies a scalar by a matrix and returns the result 
	 * as a new matrix.
	 * @param f The scalar value to multiply by.
	 * @param matrix The matrix to be multiplied.
	 * @return The result of the multiplication as a new matrix.
	 */

	friend Matrix operator*(const float &f, const Matrix &matrix);
	/**
	 * @brief Assigns the value of another matrix to this matrix.
	 * @param matrix The matrix to assign from.
	 * @return Reference to the updated matrix.
	 */
	Matrix &operator=(const Matrix &matrix);
	/**
	 * @brief Accesses the element at the specified row and column indices.
	 * @param i The row index.
	 * @param j The column index.
	 * @return The Reference (or const Reference )  at the specified position.
	 */
	float& operator()(int i, int j) ;
	const float& operator()(int i, int j) const;
	/**
	 * @brief Accesses the element at the specified index.
	 * @param i The index.
	 * @return The The Reference (or const Reference ) at the specified index.
	 */
	float& operator[](int i) ;
	const float& operator[](int i) const;

	/**
	 * @brief Overloads the insertion operator for output streams to
	 *  print the matrix.
	 * @param os The output stream.
	 * @param matrix The matrix to be printed.
	 * @return Reference to the output stream.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);
	/**
	 * @brief Overloads the extraction operator for input
	 *  streams to read the matrix.
	 * @param is The input stream.
	 * @param matrix The matrix to be read into.
	 * @return Reference to the input stream.
	 */
	friend std::istream &operator>>(std::istream &is, Matrix &matrix);
};

#endif // MATRIX_H