#ifndef BAT_SWARM_H
#define BAT_SWARM_H

#include "../include/bat.h"

class BatSwarm{
public:
    BatSwarm(int dimensions, int batCount, std::vector<double> lb, std::vector<double> ub, Function fitness);
    void update();
    Bat getBest();
    int getIterationCount(){ return currentIteration; }

private:
    int dimensions;
    int batCount;
    int currentIteration;
    std::vector<double> lb;
    std::vector<double> ub;
    Function fitness;
    std::vector<Bat> bats;
    Bat best;
};

#endif