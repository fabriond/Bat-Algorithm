#include <vector>
#include <iostream>
#include <string>
#include "../include/network.h"

using namespace std;

int main(){
    //Compile with: g++ -std=c++11 src/main.cpp src/matrix.cpp src/layer.cpp src/network.cpp src/random_engine.cpp -o main
    vector<int> topology;
    topology.push_back(3);
    topology.push_back(2);
    topology.push_back(3);

    vector<double> inputs;
    inputs.push_back(1.0);
    inputs.push_back(0.0);
    inputs.push_back(1.0);

    vector<double> weights;
    weights.push_back(0.5); weights.push_back(0.2);
    weights.push_back(0.5); weights.push_back(0.2);
    weights.push_back(0.5); weights.push_back(0.2);

    weights.push_back(0.5); weights.push_back(0.2); weights.push_back(0.5);
    weights.push_back(0.2); weights.push_back(0.5); weights.push_back(0.2);

    Network n(topology, weights);
    n.setInputs(inputs);
    /*
    Matrix m(2, 2, true);
    cout << m;
    cout << "-----------------------------" << endl;
    cout << m.transpose();
    
    m.setValue(0, 0, -1);
    cout << "-----------------------------" << endl;
    cout << m;
    */
    cout << n;
}