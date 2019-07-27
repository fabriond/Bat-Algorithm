#include <vector>
#include <iostream>
#include <string>
#include "../include/network.h"

using namespace std;

vector<double> objectiveFunction(vector<double> val){
    vector<double> v(1, sin(val[0]));
    return v;
}

Matrix expectedOutputs(vector<double> inputs){
    Matrix m(1, 1, objectiveFunction(inputs));
    return m;
}

int main(int argc, char **argv){
    //Compile with: g++ -std=c++11 src/main.cpp src/matrix.cpp src/layer.cpp src/network.cpp src/random_engine.cpp -o main
    vector<int> topology;
    topology.push_back(1);
    topology.push_back(5);
    topology.push_back(1);

    vector<double> inputs;
    inputs.push_back(1.0);

    int weightCount = 0;
    for(int i = 0; i < topology.size()-1; ++i){
        weightCount += topology.at(i)*topology.at(i+1);
    }

    vector<double> weights;
    weights.push_back(0.5); weights.push_back(0.2); weights.push_back(0.5); weights.push_back(0.2); weights.push_back(0.5);

    weights.push_back(0.5);
    weights.push_back(0.2);
    weights.push_back(0.5);
    weights.push_back(0.2); 
    weights.push_back(0.5);

    Network n(topology, weights);
    n.setInputs(inputs);
    n.feedForward();
    

    cout  << n.getOutputs() - expectedOutputs(inputs);
}