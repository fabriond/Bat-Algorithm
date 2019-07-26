#include "../include/bat.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../include/random_engine.h"

static double simpleBounds(double value, double lower, double upper){
    if(value < lower) value = lower;
    if(value > upper) value = upper;
    return value;
}

Bat::Bat(std::vector<double> initPos, double initFitness, Function fun):
    position(initPos), 
    fitness(initFitness),
    fitnessFunction(fun)
{

}

Bat::Bat(std::vector<double> initPos, std::vector<double> lb, std::vector<double> ub, Function fun):
    lowerBounds(lb), 
    upperBounds(ub), 
    fitnessFunction(fun), 
    position(initPos), 
    speed(initPos.size(), 0.0),
    frequency(uniformRandom(MIN_FREQ, MAX_FREQ)),
    initPulseRate(uniformRandom(MIN_PULSE, MAX_PULSE/2)),
    pulseRate(initPulseRate),
    loudness(uniformRandom(MAX_LOUDNESS/2, MAX_LOUDNESS))
{
    fitness = getFitness();
}

void Bat::randomWalk(){
    for(int j = 0; j < auxPosition.size(); ++j){
        auxPosition[j] += normalRandom(0.0, 1.0);
        auxPosition[j] = simpleBounds(auxPosition[j], lowerBounds[j], upperBounds[j]);
    }
}

void Bat::walk(std::vector<double> globalBestPos){

    auxPosition = std::vector<double>(position.size(), 0.0);

    frequency = MIN_FREQ + (MAX_FREQ - MIN_FREQ)*uniformRandom(0.0, 1.0);
    for(int j = 0; j < auxPosition.size(); ++j){
        speed[j] = speed[j] + (position[j] - globalBestPos[j])*frequency;
        auxPosition[j] = position[j] + speed[j];
        auxPosition[j] = simpleBounds(auxPosition[j], lowerBounds[j], upperBounds[j]);
    }
}

void Bat::aproxBest(std::vector<double> globalBestPos, double averageLoudness){
    for(int j = 0; j < auxPosition.size(); ++j){
        auxPosition[j] = globalBestPos[j] + averageLoudness * normalRandom(0.0, 1.0);
        auxPosition[j] = simpleBounds(auxPosition[j], lowerBounds[j], upperBounds[j]);
    }
}

void Bat::acceptNewSolutions(double currentIteration){
    position = auxPosition;
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