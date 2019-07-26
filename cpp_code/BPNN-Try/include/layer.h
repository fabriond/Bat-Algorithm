#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "./matrix.h"
#include "./neuron.h"

class Layer{
public:
    Layer(int neuronCount);
    void setVal(int i, double v);
    Matrix matrixifyVals() const;
    Matrix matrixifyActivatedVals() const;
    Matrix matrixifyDerivedVals() const;
    
private:
    int neuronCount;
    std::vector<Neuron> neurons;
};

#endif