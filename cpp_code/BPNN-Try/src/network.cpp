#include "../include/network.h"
#include <string>

Network::Network(std::vector<int> topology): 
    topology(topology)
{
    for(int i = 0; i < topology.size(); ++i){
        Layer l(topology.at(i));
        layers.push_back(l);
    }

    for(int i = 0; i < topology.size()-1; ++i){
        Matrix m(topology.at(i), topology.at(i+1));
        weightMatrices.push_back(m);
    }
}

void Network::setInputs(std::vector<double> inputs){
    for(int i = 0; i < inputs.size(); ++i){
        layers.at(0).setVal(i, inputs.at(i));
    }
}

std::ostream& operator<<(std::ostream& os, const Network& net){
    for(int i = 0; i < net.layers.size(); ++i){
        os << "Layer[" << i << "]: " << std::endl;
        if(i == 0) os << net.layers.at(i).matrixifyVals();
        else os << net.layers.at(i).matrixifyActivatedVals();
    }
    
    return os;
}
