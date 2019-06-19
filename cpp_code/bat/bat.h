#ifndef BAT_H
#define BAT_H

#include <functional>
#include <vector>

#define MIN_FREQ 0.0
#define MAX_FREQ 100.0

#define MIN_LOUDNESS 0.0
#define MAX_LOUDNESS 10.0

#define MIN_PULSE 0.0
#define MAX_PULSE 1.0

#define ALPHA 0.9
#define GAMMA 0.9

typedef std::function<double(std::vector<double>)> Function;

class Bat {

    std::vector<double> position;
    std::vector<double> speed;
    double frequency;
    double pulseRate;
    double loudness;
    double fitness;

    int dimensionCount;
    Function fitnessFunction;
    std::vector<double> lowerBounds;
    std::vector<double> upperBounds;
    std::vector<double> auxPosition;
    double initPulseRate;
    
    
public:
    Bat(int dimCount, std::vector<double> lb, std::vector<double> ub, Function fun);

    Bat(std::vector<double> pos, double fitness);

    void randomWalk();

    void walk(std::vector<double> globalBestPos);

    void aproxBest(std::vector<double> globalBestPos, double averageLoudness);

    void acceptNewSolutions(double currentIteration);
    
    friend std::ostream& operator<<(std::ostream& os, const Bat& bat);

    double getUpdatedFitness(){
        return fitnessFunction.operator()(auxPosition);
    }

    double getFitness(){
        return fitnessFunction.operator()(position);
    }

    double getPulseRate(){
        return pulseRate;
    }

    double getLoudness(){
        return loudness;
    }

    std::vector<double> getPosition(){
        return position;
    }

    std::vector<double> getUpdatedPosition(){
        return auxPosition;
    }
};

#endif
