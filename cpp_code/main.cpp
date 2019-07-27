#include "bat_algorithm.cpp"
#include "neural_net.cpp"
#include <cmath>
#include <limits>

int main(){
    //Compile with "g++ -std=c++11 main.cpp bat/bat.cpp random/random_engine.cpp -o main"
    int dimensions = 11;
    int batCount = 40;
    int maxIterations = 10000;
    std::vector<double> lowerBoundaries(dimensions, -100);
    std::vector<double> upperBoundaries(dimensions, 100);

    Function fitness(
        [](std::vector<double> pos) -> double {
            //return (1-pos[0]*pos[0])*(1-pos[0]*pos[0]) + 100*(pos[1]-pos[0]*pos[0])*(pos[1]-pos[0]*pos[0]);
            //return pos[0]*pos[0] + pos[1]*pos[1] + 25*(sin(pos[0])*sin(pos[0]) + sin(pos[1])*sin(pos[1]));
            return neuralNet(pos);
        }
    );
    
    batAlgorithm(dimensions, batCount, maxIterations, lowerBoundaries, upperBoundaries, fitness);
    
}