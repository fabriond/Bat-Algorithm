#include <string>
#include <fstream>
#include <iostream>
#include <direct.h>
#include "./bat/bat.h"
#include "./random/random_engine.h"

void printSolution(std::string description, Bat bat){
    std::ofstream file("../Result.txt", std::ofstream::app);
    
    file << description << " solution: " << bat << std::endl;
    file.close();
}

void plotBats(int iteration, std::vector<Bat> bats){
    /*std::ofstream file("../plots/PlotAux of iteration "+std::to_string(iteration)+".txt");

    for(Bat bat : bats){
        std::vector<double> position(bat.getPosition());
        std::string sep = "";
        for(int i = 0; i < position.size(); ++i){
            file << sep << position[i];
            sep = ", ";
        }
        file << std::endl;
    }
    file.close();*/
}

void batAlgorithm(int dimensions, int batCount, int maxIterations, std::vector<double> lb, std::vector<double> ub, Function fitness){

    std::ofstream file("../Result.txt", std::ofstream::app);
    std::vector<Bat> bats;
    
    file << "Initializing bats" << std::endl;
    
    int bestId = 0;

    for(int i = 0; i < batCount; ++i){        
        
        std::vector<double> pos(dimensions);

        for(int j = 0; j < dimensions; ++j){
            pos[j] = uniformRandom(lb[j], ub[j]);
        }
        
        Bat newBat = Bat(pos, lb, ub, fitness);
        bats.push_back(newBat);

        printSolution("Bat "+std::to_string(i)+"'s initial", bats[i]);

        if(newBat.getFitness() < bats[bestId].getFitness()){
            bestId = i;
        }
    }
    
    file << "===========================================" << std::endl;

    Bat best(bats[bestId]);

    printSolution("Initial", best);
    
    int t;
    for(t = 0;t < maxIterations && (best.getFitness() < -0.01 || best.getFitness() > 0.01); ++t){
        if(t<10){
            plotBats(t, bats);
        }
        else if(t < 100 && !(t%10)){
            plotBats(t, bats);
        }
        else if(!(t%100)){
            plotBats(t, bats);
        }

        double avgLoudness = 0;
        for(int i = 0; i < bats.size(); ++i){
            avgLoudness += bats[i].getLoudness();
        }
        avgLoudness /= bats.size();
        std::cout << avgLoudness << "\r";
        for(int i = 0; i < bats.size(); ++i){
            bats[i].walk(best.getPosition()); //Equations (2) to (4)

            if(uniformRandom(MIN_PULSE, MAX_PULSE) > bats[i].getPulseRate()){
                bats[i].aproxBest(best.getPosition(), avgLoudness); //Equation (5)
            }

            bats[i].randomWalk();

            if(uniformRandom(MIN_LOUDNESS, MAX_LOUDNESS) < bats[i].getLoudness() && bats[i].getUpdatedFitness() <= bats[i].getFitness()){
                bats[i].acceptNewSolutions(t); //Equation (6)
            }
            
            if(bats[i].getUpdatedFitness() <= best.getFitness()){ //Updating best fitness
                Bat aux(bats[i].getUpdatedPosition(), bats[i].getUpdatedFitness(), fitness);
                best = aux;
                //best = bats[i];
                printSolution("Updated", best);
            }
        }
    }

    printSolution("Final", best);

    if(t%100){
        plotBats(t, bats);
    }

    file << "===========================================" << std::endl;
    file << "Iteration Count: " << t << std::endl;
    file << "Checking on bats" << std::endl;
    for(int i = 0; i < bats.size(); ++i){
        std::string desc = "Bat "+std::to_string(i)+"'s final";
        printSolution(desc, bats[i]);
    }

    file << "+++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
    file.close();
}