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

Network::Network(std::vector<int> topology, std::vector<double> weights): 
    topology(topology)
{
    for(int i = 0; i < topology.size(); ++i){
        Layer l(topology.at(i));
        layers.push_back(l);
    }

    for(int i = 0, current = 0; i < topology.size()-1; ++i){
        auto start = weights.begin() + current;
        auto end = start + topology.at(i)*topology.at(i+1);

        std::vector<double> weightVector(start, end);
        Matrix m(topology.at(i), topology.at(i+1), weightVector);

        weightMatrices.push_back(m);
        current += topology.at(i)*topology.at(i+1);
    }
}

void Network::setInputs(std::vector<double> inputs){
    for(int i = 0; i < inputs.size(); ++i){
        layers.at(0).setVal(i, inputs.at(i));
    }
}

void Network::setWeights(std::vector<double> weights){
    for(int i = 0, current = 0; i < topology.size()-1; ++i){
        auto start = weights.begin() + current;
        auto end = start + topology.at(i)*topology.at(i+1);

        std::vector<double> weightVector(start, end);
        Matrix m(topology.at(i), topology.at(i+1), weightVector);
        
        weightMatrices.at(i) = m;
        current += topology.at(i)*topology.at(i+1);
    }
}

std::ostream& operator<<(std::ostream& os, const Network& net){
    for(int i = 0; i < net.layers.size(); ++i){
        os << "Layer[" << i << "]: " << std::endl;
        if(i == 0) os << net.layers.at(i).matrixifyVals();
        else os << net.layers.at(i).matrixifyActivatedVals();
    }

    for(int i = 0; i < net.weightMatrices.size(); ++i){
        os << "Layer[" << i << "] Weights: " << std::endl;
        os << net.weightMatrices.at(i);
    }
    
    return os;
}
