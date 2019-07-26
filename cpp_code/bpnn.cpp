#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include "./bat_swarm.cpp"
#include "./matrix/matrix.h"

double sigmoid(double x) {
	return (1.0 / (1.0 + std::exp(-x)));
}

double derivative_sigmoid(double x) {
	return (x*(1.0-x));
}

class Neuron{
public:
    Neuron(double val): val(val){ activate(); derive(); }
    void setVal(double val){ activate(); derive(); }
    void activate(){
        activatedVal = sigmoid(val);
    }
    void derive(){
        derivedVal = derivative_sigmoid(val);
    }
    double getVal(){ return val; }
    double getActivatedVal(){ return activatedVal; }
    double getDerivedVal(){ return derivedVal; }
private:
    double val;
    double activatedVal;
    double derivedVal;
};

class Layer{
public:
    Layer(int neuronCount): 
        neuronCount(neuronCount)
    {
        for(int i = 0; i < neuronCount; ++i){
            Neuron neuron(0.0);
            neurons.push_back(neuron);
        }
    }

    void setVal(int i, double v){
        neurons.at(i).setVal(v);
    }
private:
    int neuronCount;
    std::vector<Neuron> neurons;
};

class Network{
public:
    Network(std::vector<int> topology): 
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
private:
    std::vector<int> topology;
    std::vector<Layer> layers;
    std::vector<Matrix> weightMatrices;
};

class BPNN{
    Matrix weights;
    double bias;

    BPNN(Matrix weights, double bias): 
        weights(weights), 
        bias(bias)
    {
        
    }

    void train(Matrix inputs, Matrix expectedOutputs, double iterations){

        int dimensions = weights.getNumRows()*weights.getNumCols();
        int batCount = 40;
        std::vector<double> lowerBoundaries(dimensions, -1000);
        std::vector<double> upperBoundaries(dimensions, 1000);

        //BatSwarm swarm(dimensions, batCount, lowerBoundaries, upperBoundaries, );

        for(int i = 0; i < iterations; ++i){
            Matrix outputs = think(inputs);

            Matrix errors = expectedOutputs - outputs;

            Matrix adjustments = dot(inputs.transpose(), errors*outputs.forEach(derivative_sigmoid));

            weights = weights + adjustments;
        }
    }

    Matrix think(Matrix inputs){
        Matrix hiddenOutputs = dot(inputs, weights);
        return hiddenOutputs.forEach(sigmoid);
    }
    
};