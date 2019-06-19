#include "bat.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../random/random_engine.h"

Bat::Bat(int dimCount, std::vector<double> lb, std::vector<double> ub, Function fun):
dimensionCount(dimCount), lowerBounds(lb), upperBounds(ub), fitnessFunction(fun), position(dimCount), speed(dimCount){
    frequency = uniformRandom(MIN_FREQ, MAX_FREQ);
    initPulseRate = uniformRandom(MIN_PULSE, MAX_PULSE/2);
    pulseRate = initPulseRate;
    loudness = uniformRandom(MAX_LOUDNESS/2, MAX_LOUDNESS);
    for(int j = 0; j < dimensionCount; ++j){
        speed[j] = 0.0;
        position[j] = lowerBounds[j] + (upperBounds[j] - lowerBounds[j])*uniformRandom(0.0, 1.0);
    }
    fitness = getFitness();
}

void Bat::randomWalk(){
    for(int j = 0; j < dimensionCount; ++j){
        auxPosition[j] += normalRandom(0.0, 1.0);
        if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
        if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
    }
}

void Bat::walk(std::vector<double> globalBestPos){

    auxPosition = std::vector<double>(position.size(), 0.0);

    frequency = MIN_FREQ + (MAX_FREQ - MIN_FREQ)*uniformRandom(0.0, 1.0);
    for(int j = 0; j < dimensionCount; ++j){
        speed[j] = speed[j] + (position[j] - globalBestPos[j])*frequency;
        auxPosition[j] = position[j] + speed[j];
        if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
        if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
    }
}

void Bat::aproxBest(std::vector<double> globalBestPos, double averageLoudness){
    for(int j = 0; j < dimensionCount; ++j){
        auxPosition[j] = globalBestPos[j] + averageLoudness * normalRandom(0.0, 1.0);
        if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
        if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
    }
}

void Bat::acceptNewSolutions(double currentIteration){
    for(int i = 0; i < position.size(); ++i){
        position[i] = auxPosition[i];
    }
    fitness = getUpdatedFitness();
    loudness = ALPHA*loudness;
    pulseRate = initPulseRate*(1.0-exp(-GAMMA*currentIteration));
}

std::ostream& operator<<(std::ostream& os, const Bat& bat){
    std::string sep = "";
    for(int j = 0; j < bat.position.size(); ++j){
        os << sep << bat.position[j];
        sep = ", ";
    }
    os << "; Fitness: " << bat.fitness;
    return os;
}