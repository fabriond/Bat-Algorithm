#include <random>
#include <functional>
#include <time.h>
#include <cmath>
#include <iostream>

#define MIN_FREQ 0.0
#define MAX_FREQ 100.0
#define MIN_LOUDNESS 0.0
#define MAX_LOUDNESS 10.0
#define MIN_PULSE 0.0
#define MAX_PULSE 1.0

#define ALPHA 0.9
#define GAMMA 0.9

typedef std::function<long double(std::vector<double>)> Function;

static std::default_random_engine generator(time(0));

static double uniformRandom(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

static double normalRandom(double mean, double sd) {
    std::normal_distribution<double> distribution(mean, sd);
    return distribution(generator);
}

class Bat {

    std::vector<double> position;
    std::vector<double> speed;
    double frequency;
    double pulseRate;
    double loudness;
    long double fitness;

    int dimensionCount;
    Function fitnessFun;
    std::vector<double> lowerBounds;
    std::vector<double> upperBounds;
    std::vector<double> auxPosition;
    double initPulseRate;
    
    
public:
    Bat(int dimCount, std::vector<double> lb, std::vector<double> ub, Function fun):
    dimensionCount(dimCount), lowerBounds(lb), upperBounds(ub), fitnessFun(fun), position(dimCount), speed(dimCount){
        frequency = uniformRandom(MIN_FREQ, MAX_FREQ);
        initPulseRate = uniformRandom(MIN_PULSE, MAX_PULSE/2);
        pulseRate = initPulseRate;
        loudness = uniformRandom(MAX_LOUDNESS/2, MAX_LOUDNESS);
        for(int j = 0; j < dimensionCount; ++j){
            speed[j] = 0.0;
            position[j] = lowerBounds[j] + (upperBounds[j] - lowerBounds[j])*uniformRandom(0.0, 1.0);
        }
        fitness = fitnessFun.operator()(position);
    }

    void randomWalk(){
        for(int j = 0; j < dimensionCount; ++j){
            auxPosition[j] += normalRandom(0.0, 1.0);
            if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
            if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
        }
    }

    void walk(std::vector<double> globalBestPos){

        auxPosition = std::vector<double>(position.size(), 0.0);

        frequency = MIN_FREQ + (MAX_FREQ - MIN_FREQ)*uniformRandom(0.0, 1.0);
        for(int j = 0; j < dimensionCount; ++j){
            speed[j] = speed[j] + (position[j] - globalBestPos[j])*frequency;
            auxPosition[j] = position[j] + speed[j];
            if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
            if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
        }
    }

    void aproxBest(std::vector<double> globalBestPos, double averageLoudness){
        for(int j = 0; j < dimensionCount; ++j){
            auxPosition[j] = globalBestPos[j] + averageLoudness * normalRandom(0.0, 1.0);
            if(auxPosition[j] < lowerBounds[j]) auxPosition[j] = lowerBounds[j];
            if(auxPosition[j] > upperBounds[j]) auxPosition[j] = upperBounds[j];
        }
    }

    void acceptNewSolutions(double currentIteration){
        for(int i = 0; i < position.size(); ++i){
            position[i] = auxPosition[i];
        }
        fitness = getUpdatedFitness();
        loudness = ALPHA*loudness;
        pulseRate = initPulseRate*(1.0-exp(-GAMMA*currentIteration));
    }

    double getUpdatedFitness(){
        return fitnessFun.operator()(auxPosition);
    }

    double getFitness(){
        return fitness;
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

#include <string>
#include <fstream>

void printSolution(std::string description, std::vector<double> bat, double fitness){
    std::ofstream file("Result.txt", std::ofstream::app);
    
    file << description << " solution: ";
    std::string sep = "";
    for(int j = 0; j < bat.size(); ++j){
        file << sep << bat[j];
        sep = ", ";
    }
    file << "; Fitness: " << fitness << std::endl;
    file.close();
}

void printFailure(std::vector<double> bat, double fitness){
    std::ofstream file("Failures.txt", std::ofstream::app);
    
    file << "Failure's solution: ";
    std::string sep = "";
    for(int j = 0; j < bat.size(); ++j){
        file << sep << bat[j];
        sep = ", ";
    }
    file << "; Fitness: " << fitness << std::endl;
    file.close();
}

void batGraphic(int iteration, std::vector<Bat> bats){
    std::ofstream file("graphs/GraphAux"+std::to_string(iteration)+".txt");
    for(Bat bat : bats){
        file << bat.getPosition()[0] << "," << bat.getPosition()[1] << std::endl;
    }
    file.close();
}

void batAlgorithm(int dimensions, int batCount, int maxIterations, std::vector<double> lb, std::vector<double> ub, Function fitness){

    std::ofstream file("Result.txt", std::ofstream::app);
    std::vector<Bat> bats;
    
    file << "Initializing bats" << std::endl;

    for(int i = 0; i < batCount; ++i){
        Bat newBat = Bat(dimensions, lb, ub, fitness);
        bats.push_back(newBat);
    }
    
    for(int i = 0; i < bats.size(); ++i){
        std::string desc = "Bat "+std::to_string(i)+"'s initial";
        printSolution(desc, bats[i].getPosition(), bats[i].getFitness());
    }
    
    file << "===========================================" << std::endl;
    
    int bestId = 0;
    for(int i = 0; i < bats.size(); ++i){
        //change this and fitness function to find global max
        if(bats[i].getFitness() < bats[bestId].getFitness()){
            bestId = i;
        }
    }
    
    std::vector<double> best(bats[bestId].getPosition());

    double minFitness = bats[bestId].getFitness();

    printSolution("Initial", best, minFitness);
    
    int t;
    for(t = 0; minFitness < -0.1 || minFitness > 0.1; ++t){
        if(!(t%100)){
            batGraphic(t, bats);
        }
        double avgLoudness = 0;
        double avgPulseRate = 0;
        for(int i = 0; i < bats.size(); ++i){
            avgLoudness += bats[i].getLoudness();
            avgPulseRate += bats[i].getPulseRate();
        }
        avgLoudness /= bats.size();
        avgPulseRate /= bats.size();

        for(int i = 0; i < bats.size(); ++i){
            bats[i].walk(best); //Equations (2) to (4)

            //using average pulse rate instead of random seems to give better results
            if(uniformRandom(0.0, 1.0) > bats[i].getPulseRate()){
                bats[i].aproxBest(best, avgLoudness); //Equation (5)
            }

            bats[i].randomWalk();

            if(uniformRandom(0.0, 1.0) < bats[i].getLoudness() && bats[i].getUpdatedFitness() <= bats[i].getFitness()){
                bats[i].acceptNewSolutions(t); //Equation (6)
            }

            if(bats[i].getUpdatedFitness() <= minFitness){ //Updating best fitness                
                for(int j = 0; j < dimensions; ++j){
                    best[j] = bats[i].getUpdatedPosition()[j];
                }
                minFitness = bats[i].getUpdatedFitness();
                printSolution("Updated", best, minFitness);
            }
        }
    }

    printSolution("Final", best, minFitness);

    file << "Iteration Count" << t << std::endl;
    batGraphic(t, bats);
    file << "===========================================" << std::endl;
    file << "Checking on bats" << std::endl;
    for(int i = 0; i < bats.size(); ++i){
        std::string desc = "Bat "+std::to_string(i)+"'s final";
        printSolution(desc, bats[i].getPosition(), bats[i].getFitness());
    }

    file << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
    file.close();
}

int main(){
    //Compile with "g++ -std=c++11 bat.cpp -o bat"
    int dimensions = 2;
    int batCount = 20;
    int maxIterations = 1000;
    std::vector<double> lowerBoundaries(dimensions, -100);
    std::vector<double> upperBoundaries(dimensions, 100);
    
    Function fitness(
        [](std::vector<double> pos) -> long double {
            //return (1-pos[0]*pos[0])*(1-pos[0]*pos[0]) + 100*(pos[1]-pos[0]*pos[0])*(pos[1]-pos[0]*pos[0]);
            return pos[0]*pos[0] + pos[1]*pos[1] + 25*(sin(pos[0])*sin(pos[0]) + sin(pos[1])*sin(pos[1]));
        }
    );

    for(int i = 0; i < 1; ++i){
        batAlgorithm(dimensions, batCount, maxIterations, lowerBoundaries, upperBoundaries, fitness);
    }
}