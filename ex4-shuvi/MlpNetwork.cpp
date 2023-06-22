#include "MlpNetwork.h"
#include "Dense.h"
#define LAST_LAYER 3

MlpNetwork::MlpNetwork(Matrix weights[], Matrix biases[])
{
    for (int i = 0; i < LAST_LAYER; i++)
    {
        layers[i] = Dense(weights[i], biases[i], activation::relu);
    }
    layers[LAST_LAYER] = Dense(weights[LAST_LAYER], biases[LAST_LAYER],
                               activation::softmax);
}

digit MlpNetwork::operator()(const Matrix &input_img) const
{
    digit result;
    Matrix output = input_img;
    output.vectorize();

    for (int i = 0; i < MLP_SIZE; i++)
    {
        Matrix input = output;
        output = layers[i](input);
    }
    result.value = output.argmax();
    result.probability = output[result.value];
    return result;
}