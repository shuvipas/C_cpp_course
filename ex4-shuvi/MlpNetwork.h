// MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit
{
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64, 128},
									{20, 64},
									{10, 20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64, 1},
								 {20, 1},
								 {10, 1}};

/**
 * @class MlpNetwork
 * @brief Represents a Multi-Layer Perceptron (MLP) network.
 */
class MlpNetwork
{
private:
enum LayerNum
{
    R1,
    R2,
    R3,
    R4
};
	Dense layers[MLP_SIZE] = {
		Dense(Matrix(weights_dims[R1].rows, weights_dims[R1].cols),
		Matrix(bias_dims[R1].rows, bias_dims[R1].cols), activation::relu),
		Dense(Matrix(weights_dims[R2].rows, weights_dims[R2].cols),
		Matrix(bias_dims[R2].rows, bias_dims[R2].cols), activation::relu),
		Dense(Matrix(weights_dims[R2].rows, weights_dims[R2].cols),
		Matrix(bias_dims[R3].rows, bias_dims[R3].cols), activation::relu),
		Dense(Matrix(weights_dims[3].rows, weights_dims[3].cols),
		Matrix(bias_dims[R4].rows, bias_dims[R4].cols), activation::softmax)};
		

public:
	/**
	 * @brief Constructor for MlpNetwork class.
	 * @param weights An array of weight matrices for each layer.
	 * @param biases An array of bias matrices for each layer.
	 */
	MlpNetwork(Matrix weights[], Matrix biases[]);
	/**
	 * @brief Applies the MLP network to an input image matrix and
	 *  returns the identified digit.
	 * @param input_img The input image matrix.
	 * @return The identified digit and its probability.
	 */
	digit operator()(const Matrix &input_img) const;
};

#endif // MLPNETWORK_H