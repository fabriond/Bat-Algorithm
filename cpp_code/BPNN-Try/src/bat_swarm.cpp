#include <string>
#include <fstream>
#include <direct.h>
#include "../include/bat.h"
#include "../include/random_engine.h"

void printSolution(std::string description, Bat bat){
    std::ofstream file("../Result.txt", std::ofstream::app);
    
    file << description << " solution: " << bat << std::endl;
    file.close();
}

class BatSwarm{
    int dimensions;
    int batCount;
    int currentIteration;
    std::vector<double> lb;
    std::vector<double> ub;
    Function fitness;
    std::vector<Bat> bats;
    Bat best;
public:
    BatSwarm(int dimensions, int batCount, std::vector<double> lb, std::vector<double> ub, Function fitness):
        dimensions(dimensions), 
        batCount(batCount), 
        currentIteration(0),
        lb(lb), 
        ub(ub), 
        fitness(fitness),
        best({}, {}, fitness)
    {
        int bestId = 0;

        for(int i = 0; i < batCount; ++i){        
            std::vector<double> pos(dimensions);

            for(int j = 0; j < dimensions; ++j){
                pos[j] = uniformRandom(lb[j], ub[j]);
            }
            
            Bat newBat(pos, lb, ub, fitness);
            bats.push_back(newBat);

            if(newBat.getFitness() < bats[bestId].getFitness()){
                bestId = i;
            }
        }

        best = bats[bestId];
    }

    void update(){
        double avgLoudness = 0;
        for(int i = 0; i < bats.size(); ++i){
            avgLoudness += bats[i].getLoudness();
        }
        avgLoudness /= bats.size();

        for(int i = 0; i < bats.size(); ++i){
            bats[i].walk(best.getPosition()); //Equations (2) to (4)

            if(uniformRandom(MIN_PULSE, MAX_PULSE) > bats[i].getPulseRate()){
                bats[i].aproxBest(best.getPosition(), avgLoudness); //Equation (5)
            }

            bats[i].randomWalk();

            if(uniformRandom(MIN_LOUDNESS, MAX_LOUDNESS) < bats[i].getLoudness() && bats[i].getUpdatedFitness() <= bats[i].getFitness()){
                bats[i].acceptNewSolutions(currentIteration); //Equation (6)
            }
            
            if(bats[i].getUpdatedFitness() <= best.getFitness()){ //Updating best fitness
                Bat aux(bats[i].getUpdatedPosition(), bats[i].getUpdatedFitness(), fitness);
                best = aux;
                printSolution("Updated", best);
            }
        }
        ++currentIteration;
    }

    std::vector<double> getBest(){
        return best.getPosition();
    }
};