#include "../include/network.h"
#include <string>
#include <sstream>

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
    int expectedWeightsSize = 0;
    for(int i = 0; i < topology.size(); ++i){
        Layer l(topology.at(i));
        layers.push_back(l);
        if(i < topology.size()-1){
            expectedWeightsSize += topology.at(i)*topology.at(i+1);
        }
    }

    if(weights.size() != expectedWeightsSize){
        std::stringstream ss;
        ss << "Too many weights for network, "<< weights.size() <<" weights found, "<< expectedWeightsSize <<" expected";
        throw std::logic_error(ss.str());
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

void Network::feedForward(){
    for(int i = 0; i < layers.size()-1; ++i){
        Matrix a = layers.at(i).matrixifyVals();
        if(i != 0){
            a = layers.at(i).matrixifyActivatedVals();
        }

        Matrix b = weightMatrices.at(i);

        Matrix c = dot(a, b);

        for(int j = 0; j < c.getNumCols(); ++j){
            setNeuronVal(i+1, j, c.getValue(0, j));
        }
    }
}

Matrix Network::getOutputs(){
    return layers.back().matrixifyActivatedVals();
}

void Network::setInputs(std::vector<double> inputs){
    int inputLayerSize = layers.at(0).matrixifyVals().getNumCols();
    
    if(inputs.size() > inputLayerSize){
        std::stringstream ss;
        ss << "Too many inputs for network, "<< inputs.size() <<" inputs found, "<< inputLayerSize <<" expected";
        throw std::logic_error(ss.str());
    }

    for(int i = 0; i < inputs.size(); ++i){
        layers.at(0).setVal(i, inputs.at(i));
    }
}

void Network::setWeights(std::vector<double> weights){
    int expectedWeightsSize = 0;
    for(int i = 0; i < topology.size()-1; ++i){
        expectedWeightsSize += topology.at(i)*topology.at(i+1);
    }

    if(weights.size() != expectedWeightsSize){
        std::stringstream ss;
        ss << "Too many weights for network, "<< weights.size() <<" weights found, "<< expectedWeightsSize <<" expected";
        throw std::logic_error(ss.str());
    }

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
        if(i < net.layers.size()-1){
            os << std::endl;
            os << "Layer[" << i << "] Weights: " << std::endl;
            os << net.weightMatrices.at(i) << std::endl << std::endl;
        }
    }
    
    return os;
}
