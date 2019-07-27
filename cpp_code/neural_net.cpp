#include <cmath>
#include <vector>
#include <iostream>
#include "./random/random_engine.h"
 
using namespace std;
 
#define Train_Set_Size 200
#define PI 3.141592653589793238463
#define Input_Count 1
#define Hidden_Count 5
#define Output_Count 1

vector<double> weightsIH(Input_Count*Hidden_Count);
vector<double> weightsHO(Hidden_Count*Output_Count);
double bias;

/*
Camada oculta e 5 neurônios
Função Sigmoid para a ativação de cada nó
*/
double sigmoid(double x){
    return (x/(1.0+abs(x)));
}

double runNeuron(vector<double> inputs, vector<double> weights){
    double sum = 0.0;
    for(int i = 0; i < inputs.size(); ++i){
        sum += weights.at(i) * inputs.at(i) + bias;
    }
    return sigmoid(sum);
}

vector<double> run(vector<double> inputs){
    vector<double> hiddenOutputs;
    for(int i = 0; i < Hidden_Count; ++i){
        auto startPos = weightsIH.begin() + i*Input_Count;
        auto endPos = startPos + Input_Count;
        vector<double> weights(startPos, endPos);
        hiddenOutputs.push_back(runNeuron(inputs, weights));
    }

    vector<double> outputs;
    for(int i = 0; i < Output_Count; ++i){
        auto startPos = weightsHO.begin() + i*Hidden_Count;
        auto endPos = startPos + Hidden_Count;
        vector<double> weights(startPos, endPos);
        outputs.push_back(runNeuron(hiddenOutputs, weights));
    }

    return outputs;
}

double calcError(vector<double> outputs, vector<double> expected){
    double error = 0.0;
    for(int i = 0; i < expected.size(); ++i){
        error += (outputs.at(i) - expected.at(i))*(outputs.at(i) - expected.at(i));
    }
    return error;
}
 
double neuralNet(vector<double> initWeights = {-0.430404, 31.3573, -34.8103, -84.4323, -88.6986, 7.34457, 91.7602, 6.59442, 4.63957, 85.5925, 1.24908}){
    
    vector<double> wIH(initWeights.begin(), initWeights.begin() + weightsIH.size());
    vector<double> wHO(initWeights.begin() + weightsIH.size(), initWeights.end()-1);
    bias = initWeights.back();
    
    weightsIH = wIH;
    weightsHO = wHO;
    
    double totalError = 0.0;
    for(int i=0;i<Train_Set_Size;i++){
        vector<double> inputs;
        vector<double> expected;
        inputs.push_back(i*2*PI/Train_Set_Size);
        expected.push_back(sin(i*2*PI/Train_Set_Size));
        totalError += calcError(run(inputs), expected);
    }
    totalError /= Train_Set_Size;
    //cout << totalError << endl;
    /*
    vector<double> inputs;
    vector<double> expected;
    double i = uniformRandom(0, Train_Set_Size);
    inputs.push_back(i*2*PI/Train_Set_Size);
    expected.push_back(sin(i*2*PI/Train_Set_Size));
    double totalError = calcError(run(inputs), expected);
    */
    //Plot the results
	vector<float> x;
	vector<float> y1, y2;

	for (int i = 0; i < 1000; i++) {
        vector<double> inputs;
        inputs.push_back(i * 2 * PI / 1000);
		x.push_back(i * 2 * PI / 1000);
		y1.push_back(sin(i * 2 * PI / 1000));
		y2.push_back(run(inputs)[0]);
	}

	FILE * gp = fopen("gnuplot.plt", "w");
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


	system("pause");
	_pclose(gp);

    return totalError;
}

int main(){
    neuralNet();
    //*/return totalError;
}