#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"

// Insert Activation class here...
/**
 * @namespace activation
 * Contains activation functions for neural networks.
 */
namespace activation
{
     /**
     * @brief Applies the Rectified Linear Unit (ReLU) activation function
     *  element-wise to a matrix.
     * @param matrix The input matrix.
     * @return The matrix with the ReLU activation applied element-wise.
     */
    Matrix relu(const Matrix& matrix);

    /**
     * @brief Applies the Softmax activation function element-wise to a matrix.
     * @param matrix The input matrix.
     * @return The matrix with the Softmax activation applied element-wise.
     */
    Matrix softmax(const Matrix& matrix);
}
#endif //ACTIVATION_H