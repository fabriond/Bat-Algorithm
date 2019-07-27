#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "../include/layer.h"

class Network{
public:
    Network(std::vector<int> topology);
    Network(std::vector<int> topology, std::vector<double> weights);
    void setInputs(std::vector<double> inputs);
    void setWeights(std::vector<double> weights);
    void feedForward();
    Matrix getOutputs();
    void setNeuronVal(int idLayer, int idNeuron, double val){ layers.at(idLayer).setVal(idNeuron, val); }
    friend std::ostream& operator<<(std::ostream& os, const Network& net);
    
private:
    std::vector<int> topology;
    std::vector<Layer> layers;
    std::vector<Matrix> weightMatrices;
};

#endif