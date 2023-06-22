#include "Dense.h"

Dense::Dense(const Matrix &w, const Matrix &b,
      const activation_function &act) :
      weights(w), bias(b), activation(act ){}

Matrix Dense::operator()(const Matrix &input) const
{
    Matrix output = (weights * input) + bias;
    return activation(output);
}