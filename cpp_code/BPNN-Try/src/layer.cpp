#include "../include/layer.h"
#include <iostream>
#include <string>
Layer::Layer(int neuronCount): 
    neuronCount(neuronCount)
{
    for(int i = 0; i < neuronCount; ++i){
        Neuron neuron(0.0);
        neurons.push_back(neuron);
    }
}

void Layer::setVal(int i, double v){
    neurons.at(i).setVal(v);
}

Matrix Layer::matrixifyVals() const {
    Matrix m(1, neurons.size());
    for(int i = 0; i < neurons.size(); ++i){
        m.setValue(0, i, neurons.at(i).getVal());
    }
    return m;
}

Matrix Layer::matrixifyActivatedVals() const {
    Matrix m(1, neurons.size());
    for(int i = 0; i < neurons.size(); ++i){
        m.setValue(0, i, neurons.at(i).getActivatedVal());
    }
    return m;
}

Matrix Layer::matrixifyDerivedVals() const {
    Matrix m(1, neurons.size());
    for(int i = 0; i < neurons.size(); ++i){
        m.setValue(0, i, neurons.at(i).getDerivedVal());
    }
    return m;
}