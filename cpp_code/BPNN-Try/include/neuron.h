#ifndef NEURON_H
#define NEURON_H

#include <math.h>

static double sigmoid(double x) {
	return (1.0 / (1.0 + std::exp(-x)));
}

static double derivative_sigmoid(double x) {
	return (x*(1.0-x));
}

class Neuron{
public:
    Neuron(double val): val(val){ activate(); derive(); }
    void setVal(double v){ val = v; activate(); derive(); }
    double getVal() const { return val; }
    double getActivatedVal() const { return activatedVal; }
    double getDerivedVal() const { return derivedVal; }
private:
    double val;
    double activatedVal;
    double derivedVal;
    void activate(){ activatedVal = sigmoid(val); }
    void derive(){ derivedVal = derivative_sigmoid(val); }
};

#endif