#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "../include/network.h"
#include "../include/bat_swarm.h"

using namespace std;

#define Train_Set_Size 50
#define PI 3.141592653589793238463

vector<int> topology;
vector<vector<double> > trainSet;

vector<double> objectiveFunction(vector<double> val){
    vector<double> v(1, sin(val[0]));
    return v;
}

Matrix expectedOutputs(vector<double> inputs){
    Matrix m(1, 1, objectiveFunction(inputs));
    return m;
}

double fitness(vector<double> pos){
    Network n(topology, pos);
    double mse = 0.0;
    for(int i = 0; i < trainSet.size(); ++i){
        double error = (n.feedForward(trainSet.at(i)) - expectedOutputs(trainSet.at(i))).getValue(0,0);
        mse += error*error;
    }
    return mse/trainSet.size();
}
void genGraph(vector<double> pos, int number);
Bat train(){
    for(int i=0;i<Train_Set_Size;i++){
        vector<double> inputs;
        inputs.push_back(i*2*PI/Train_Set_Size);
        //Add more inputs here if necessary
        trainSet.push_back(inputs);
    }

    int weightCount = 0;
    for(int i = 0; i < topology.size()-1; ++i){
        weightCount += topology.at(i)*topology.at(i+1);
    }

    vector<double> lb(weightCount, -100);
    vector<double> ub(weightCount, 100);

    BatSwarm swarm(weightCount, 40, lb, ub, fitness);
    Bat best = swarm.getBest();
    
    while(0.01 < best.getFitness() && swarm.getIterationCount() < 2000){
        swarm.update();
        if(swarm.getBest() <= best){
            if(swarm.getBest() < best){
                genGraph(swarm.getBest().getPosition(), swarm.getIterationCount());
                cout << "Iterations: " << swarm.getIterationCount() << "; " << swarm.getBest() << "\r";
            }
            best = swarm.getBest();
        }
    }

    cout << "Iterations: " << swarm.getIterationCount() << "; " << best << endl;
    return best;
}

void genGraph(vector<double> pos, int number){
    Network n(topology, pos);
    //Plot the results
	vector<float> x;
	vector<float> y1, y2;

	for (int i = 0; i < 1000; i++) {
        vector<double> inputs;
        inputs.push_back(i * 2 * PI / 1000);
		x.push_back(i * 2 * PI / 1000);
		y1.push_back(expectedOutputs(inputs).getValue(0,0));
		y2.push_back(n.feedForward(inputs).getValue(0,0));
	}

    std::stringstream ss;
    ss << "./plots/gnuplot"<< number <<".plt";
    string filepath = ss.str();

	FILE * gp = fopen(filepath.c_str(), "w");
    fprintf(gp, "set terminal wxt size 600,400 \n");
    fprintf(gp, "set grid \n");
    fprintf(gp, "set title '%s' \n", "f(x) = sin (x)");
    fprintf(gp, "set style line 1 lt 3 pt 7 ps 0.1 lc rgb 'green' lw 1 \n");
    fprintf(gp, "set style line 2 lt 3 pt 7 ps 0.1 lc rgb 'red' lw 1 \n");
    fprintf(gp, "plot '-' w p ls 1, '-' w p ls 2 \n");

	//Exact f(x) = sin(x) -> Green Graph
	for (int k = 0; k < x.size(); k++) {
		fprintf(gp, "%f %f \n", x[k], y1[k]);
	}
	fprintf(gp, "e\n");

	//Neural Network Approximate f(x) = sin(x) -> Red Graph
	for (int k = 0; k < x.size(); k++) {
		fprintf(gp, "%f %f \n", x[k], y2[k]);
	}
	fprintf(gp, "e\n");
	
	fflush(gp);

	fclose(gp);
}

int main(int argc, char **argv){
    //Compile with: g++ -std=c++11 src/main.cpp src/matrix.cpp src/layer.cpp src/network.cpp src/random_engine.cpp src/bat_swarm.cpp src/bat.cpp -o main
    topology.push_back(1);
    topology.push_back(5);
    topology.push_back(1);

    train();
}