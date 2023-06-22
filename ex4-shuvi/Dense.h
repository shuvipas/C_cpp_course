#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"
typedef Matrix (*activation_function)(const Matrix &);

/**
 * @class Dense
 * @brief Represents a dense layer in a neural network.
 */
class Dense
{
private:
    Matrix weights;
    Matrix bias;
    activation_function activation;

public:
    /**
     * @brief Constructor for Dense class.
     * @param weights The weight matrix of the layer.
     * @param bias The bias matrix of the layer.
     * @param activation The activation function of the layer.
     */
    Dense(const Matrix &w, const Matrix &b,
          const activation_function &act);

    Matrix get_weights() const { return weights; }
    Matrix get_bias() const { return bias; }
    /**
     * @brief Retrieves the activation function of the layer.
     * @return The activation function.
     */
    activation_function get_activation() const { return activation; }

    /**
     * @brief Applies the dense layer to an input matrix.
     * @param input The input matrix.
     * @return The result of applying the dense layer to the input matrix.
     */
    Matrix operator()(const Matrix &input) const;
};

#endif // DENSE_H
