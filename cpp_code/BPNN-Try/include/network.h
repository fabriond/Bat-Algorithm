#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "../include/layer.h"

class Network{
public:
    Network(std::vector<int> topology);
    void setInputs(std::vector<double> inputs);
    void setWeights(std::vector<double> weights);
    friend std::ostream& operator<<(std::ostream& os, const Network& net);
    
private:
    std::vector<int> topology;
    std::vector<Layer> layers;
    std::vector<Matrix> weightMatrices;
};

#endif