// NeuralNetwork.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<iostream>
#include<cmath>
#include<ctime>

using namespace std;
/*float RandomWeight(float weight) 
{
srand(time(NULL));
weight=(1.0*(rand() % 1000) / 100)-5;
return weight;
}
*/



double BipolarSigmoid(double x)
{ 
	return (2 / (1 + exp(-x))) - 1;
}

double BipolarSigmoidDerivative(double x)
{
	return ((1 + ((2 / (1 + exp(-x))) - 1))*(1 - ((2 / (1 + exp(-x))) - 1))) / 2;
	
}

double Gradient(double out, double delta)
{
	double gradient = out * delta;
	return gradient;
}


int main()
{
	srand(time(NULL));
	double ExampleERMS = 0;
	double RMSE = 0;
	double Innertion = 0;
	double LearningRate = 0;
	//1d array - neurons on input layer
	double *InputNeuro = new double[500];
	//1d array - neurons on output layer
	double *OutputNeuro = new double[500];
	//2d array - hidden layers and neurons on hidden layer
	double **LayerNeuro = new double*[50];
	for (int i = 0; i < 50; i++) LayerNeuro[i] = new double[500];
	//1d array
	double *DeltaErrorOutput = new double[500];
	//2d array
	double **DeltaErrorLayerNeuro = new double*[50];
	for (int i = 0; i < 50; i++) DeltaErrorLayerNeuro[i] = new double[500];
	//2d array
	double **OutputWeightsNeuro = new double*[500];
	for (int i = 0; i < 500; i++) OutputWeightsNeuro[i] = new double[500];
	//3d array
	double ***WeightsNeuro = new double**[50];
	for (int i = 0; i < 50; i++) WeightsNeuro[i] = new double*[500];
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			WeightsNeuro[i][j] = new double[500];
		}
	}
	//2d array
	double **DeltaWeightsOutput = new double*[50];
	for (int i = 0; i < 50; i++) DeltaWeightsOutput[i] = new double[500];
	//3d array
	double ***DeltaWeightsNeuro = new double**[50];
	for (int i = 0; i < 50; i++) DeltaWeightsNeuro[i] = new double*[500];
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			DeltaWeightsNeuro[i][j] = new double[500];
		}
	}
	//2d array
	double **InputVector = new double*[1000];
	for (int i = 0; i < 50; i++) InputVector[i] = new double[500];
	//2d array
	double **TrueAnswer = new double*[1000];
	for (int i = 0; i < 50; i++) TrueAnswer[i] = new double[500];
	int InputNeuroNumber = 0;
	int OutputNeuroNumber = 0;

	int HidenNeuroNumber[50];
	int HidenLayersNumber = 0;
	int ExampleNumber = 0;
	int IterationNumber = 0;


	
	cout << "\nEnter number of Input Neurons (1-500): ";
	cin >> InputNeuroNumber;
	InputNeuroNumber--;

	cout << "\nEnter number of hiden Neuron layers (1-50): ";
	cin >> HidenLayersNumber;
	HidenLayersNumber--;

	for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
	{
		cout << "\n\tEnter number of Neurons in layer " << HidenLayersClock + 1 << " (1-500): ";
		cin >> HidenNeuroNumber[HidenLayersClock];
		HidenNeuroNumber[HidenLayersClock]--;
	}

	cout << "\nEnter number of Output Neurons (1-500): ";
	cin >> OutputNeuroNumber;
	OutputNeuroNumber--;

	cout << "\nEnter number of training Iterations: ";
	cin >> IterationNumber;
	IterationNumber--;

	cout << "\nEnter number of training Examples (1-1000): ";
	cin >> ExampleNumber;
	ExampleNumber--;

	for (int ExampleClock = 0; ExampleClock <= ExampleNumber; ExampleClock++)
	{
		cout << "\n\nInput Vector for training example " << ExampleClock + 1 << "\n\n";
			for (int InputVectorClock = 0; InputVectorClock <= InputNeuroNumber; InputVectorClock++)
			{
				cout << "\nInput Vector volume for neuron " << InputVectorClock + 1<<": ";
				cin >> InputVector[ExampleClock][InputVectorClock];

			}
			cout << "\n\nTrue Vector for training example " << ExampleClock + 1 << "\n\n";
			for (int TrueAnswerVectorClock = 0; TrueAnswerVectorClock <= OutputNeuroNumber; TrueAnswerVectorClock++)
			{
				cout << "\nTrue Vector volume for neuron " << TrueAnswerVectorClock + 1<<": ";
				cin >> TrueAnswer[ExampleClock][TrueAnswerVectorClock];

			}
	}

	cout << "\nEnter Learning Rate < 1 \n";
	cin >> LearningRate;

	cout << "\nEnter Innertion < 1 \n";
	cin >> Innertion;

	cout << "\nEnter RMSE < 1 \n";
	cin >> RMSE;

	//Random Weights
	
	
	for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
	{
		if (HidenLayersClock == 0)
		{ 
			for (int InputLayerClock = 0; InputLayerClock <= InputNeuroNumber; InputLayerClock++)
			{
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					WeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] = (1.0*(rand() % 1000) / 100) - 5;
				}
			}
		}
		else if (HidenLayersClock > 0)
		{
			for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock-1]; PeviousHidenNeuroClock++)
			{
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					WeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] = (1.0*(rand() % 1000) / 100) - 5;
				}
			}
		}
		
		
	}

	for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersNumber]; HidenNeuroClock++)
	{
		for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
		{
			OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] = (1.0*(rand() % 1000) / 100) - 5;
		}
	}

	//Checking Weights

	for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
	{
		if (HidenLayersClock == 0)
		{ 
			cout << "\n\nInput layer to 1st hiden layer Weights\n";
			for (int InputLayerClock = 0; InputLayerClock <= InputNeuroNumber; InputLayerClock++)
			{
				cout << "Input Neuron number " << InputLayerClock + 1 << " weights:";
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					cout << " " <<  WeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] << " |";
				}
				cout << endl;
			}
		}
		else if (HidenLayersClock > 0)
		{ 
			cout << "\n\nHiden layer "<< HidenLayersClock<<" to hiden layer "<< HidenLayersClock +1<<" Weights\n";
			for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock - 1]; PeviousHidenNeuroClock++)
			{
				cout << "Neuron number " << PeviousHidenNeuroClock + 1 << " weights:";
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					cout << " " << WeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] << " |";
				}
				cout << endl;
			}
		}
		
	}

	cout << "\n\nHiden layer " << HidenLayersNumber+1 << " to Output layer Weights\n";
	for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersNumber]; HidenNeuroClock++)
	{   
		cout << "Neuron number " << HidenNeuroClock + 1 << " weights:";
		for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
		{
			cout << " " << OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock]<<" |";
		}
		cout << endl;
	}

	//Nulling DeltaWeights
	for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
	{
		if (HidenLayersClock == 0)
		{
			for (int InputLayerClock = 0; InputLayerClock <= InputNeuroNumber; InputLayerClock++)
			{
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					DeltaWeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] = 0;
				}
			}
		}
		else if (HidenLayersClock > 0)
		{
			for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock - 1]; PeviousHidenNeuroClock++)
			{
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
				{
					DeltaWeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] = 0;
				}
			}
		}


	}

	for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersNumber]; HidenNeuroClock++)
	{
		for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
		{
			OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] = 0;
		}
	}


	//Learning cycle



	for (int Iteration = 0; Iteration <= IterationNumber; Iteration++)
	{
		cout << "\n\nIteration number " << Iteration + 1 << endl << endl;
		for (int ExampleClock = 0; ExampleClock <= ExampleNumber; ExampleClock++)
		{
			cout << "\n\nTraining Example number " << ExampleClock + 1 << endl << endl;
			//Main Neural Calculations
			for (int InputNeuralClock = 0; InputNeuralClock <= InputNeuroNumber; InputNeuralClock++)
			{
				InputNeuro[InputNeuralClock] = InputVector[ExampleClock][InputNeuralClock];
			}

			for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
			{
				if (HidenLayersClock == 0)
				{
					cout << "\n\nHidden Layer " << HidenLayersClock + 1 << " neurons out:\n\n";
					for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
					{
						for (int InputNeuroClock = 0; InputNeuroClock <= InputNeuroNumber; InputNeuroClock++)
						{
							LayerNeuro[HidenLayersClock][HidenNeuroClock] = LayerNeuro[HidenLayersClock][HidenNeuroClock] + (WeightsNeuro[HidenLayersClock][InputNeuroClock][HidenNeuroClock] * InputNeuro[InputNeuroClock]);
							
						}
						LayerNeuro[HidenLayersClock][HidenNeuroClock] = BipolarSigmoid(LayerNeuro[HidenLayersClock][HidenNeuroClock]);
						cout << "Neuron number " << HidenNeuroClock + 1 << " = " << LayerNeuro[HidenLayersClock][HidenNeuroClock] << endl;
					}
				}
				else if (HidenLayersClock > 0)
				{
					cout << "\n\nHidden Layer " << HidenLayersClock + 1 << " neurons out:\n\n";
					for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
					{
						for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock - 1]; PeviousHidenNeuroClock++)
						{
							LayerNeuro[HidenLayersClock][HidenNeuroClock] = LayerNeuro[HidenLayersClock][HidenNeuroClock] + (WeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] * LayerNeuro[HidenLayersClock - 1][PeviousHidenNeuroClock]);
						}
						LayerNeuro[HidenLayersClock][HidenNeuroClock] = BipolarSigmoid(LayerNeuro[HidenLayersClock][HidenNeuroClock]);
						cout << "Neuron number " << HidenNeuroClock + 1 << " = " << LayerNeuro[HidenLayersClock][HidenNeuroClock] << endl;
					}
				}
			}

			cout << "\n\nOutput Layer neurons out:\n";
			for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
			{
				
				for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersNumber]; HidenNeuroClock++)
				{
					OutputNeuro[OutputNeuroClock] = OutputNeuro[OutputNeuroClock]+(OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] * LayerNeuro[HidenLayersNumber][HidenNeuroClock]);
					
				}
				OutputNeuro[OutputNeuroClock] = BipolarSigmoid(OutputNeuro[OutputNeuroClock]);
				cout << "Neuron number " << OutputNeuroClock + 1 << " = " << OutputNeuro[OutputNeuroClock] << endl;
			}

			//Delta Error back propagation
			cout << "\n\nOutput Layer neurons Delta:\n\n";
			for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
			{
				DeltaErrorOutput[OutputNeuroClock] = (TrueAnswer[ExampleClock][OutputNeuroClock] - OutputNeuro[OutputNeuroClock])*BipolarSigmoidDerivative(OutputNeuro[OutputNeuroClock]);
				cout << "Neuron number " << OutputNeuroClock + 1 << " Delta = " << DeltaErrorOutput[OutputNeuroClock] << endl;
			}

			for (int HidenLayersClock = HidenLayersNumber; HidenLayersClock >= 0; HidenLayersClock--)
			{
				if (HidenLayersClock == HidenLayersNumber)
				{
					cout << "\n\nHidden Layer " << HidenLayersClock + 1 << " neurons Delta:\n\n";
					for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
					{
						DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = 0;
						for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
						{
							
							DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] + (OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] * DeltaErrorOutput[OutputNeuroClock]);
							
						}
						
						DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = BipolarSigmoidDerivative(LayerNeuro[HidenLayersClock][HidenNeuroClock])*DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock];
						cout << "Neuron number " << HidenNeuroClock + 1 << " Delta = " << DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] << endl;
						//cout << DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock];
					}
				}
				else if (HidenLayersClock < HidenLayersNumber)
				{
					cout << "\n\nHidden Layer " << HidenLayersClock + 1 << " neurons Delta:\n\n";
					for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
					{
						DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = 0;
						for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock + 1]; PeviousHidenNeuroClock++)
						{
							
							DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] + (WeightsNeuro[HidenLayersClock+1][HidenNeuroClock][PeviousHidenNeuroClock] * DeltaErrorLayerNeuro[HidenLayersClock + 1][PeviousHidenNeuroClock]);
							//cout << DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock];
						}
						DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] = BipolarSigmoidDerivative(LayerNeuro[HidenLayersClock][HidenNeuroClock])*DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock];
						cout << "Neuron number " << HidenNeuroClock + 1 << " Delta = " << DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock] << endl;
					}
				}
			}
			//DeltaWeights Calculation & Weights correction

			for (int HidenLayersClock = 0; HidenLayersClock <= HidenLayersNumber; HidenLayersClock++)
			{
				if (HidenLayersClock == 0)
				{
					for (int InputLayerClock = 0; InputLayerClock <= InputNeuroNumber; InputLayerClock++)
					{
						for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
						{
							
							DeltaWeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] = (LearningRate* Gradient(InputNeuro[InputLayerClock], DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock]))+ (DeltaWeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock]*Innertion);
							WeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] = WeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock] + DeltaWeightsNeuro[HidenLayersClock][InputLayerClock][HidenNeuroClock];
						}
					}
				}
				else if (HidenLayersClock > 0)
				{
					for (int PeviousHidenNeuroClock = 0; PeviousHidenNeuroClock <= HidenNeuroNumber[HidenLayersClock - 1]; PeviousHidenNeuroClock++)
					{
						for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersClock]; HidenNeuroClock++)
						{
							DeltaWeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] = (LearningRate* Gradient(LayerNeuro[HidenLayersClock-1][PeviousHidenNeuroClock], DeltaErrorLayerNeuro[HidenLayersClock][HidenNeuroClock])) + (DeltaWeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] * Innertion);
							WeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] = WeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock] + DeltaWeightsNeuro[HidenLayersClock][PeviousHidenNeuroClock][HidenNeuroClock];
						}
					}
				}


			}

			for (int HidenNeuroClock = 0; HidenNeuroClock <= HidenNeuroNumber[HidenLayersNumber]; HidenNeuroClock++)
			{
				for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
				{
					DeltaWeightsOutput[HidenNeuroClock][OutputNeuroClock] = (LearningRate*Gradient(LayerNeuro[HidenLayersNumber][HidenNeuroClock],DeltaErrorOutput[OutputNeuroClock]))+(DeltaWeightsOutput[HidenNeuroClock][OutputNeuroClock]*Innertion);
					OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] = OutputWeightsNeuro[HidenNeuroClock][OutputNeuroClock] + DeltaWeightsOutput[HidenNeuroClock][OutputNeuroClock];
				}
			}

			//Checking ERMS
			ExampleERMS = 0;
			for (int OutputNeuroClock = 0; OutputNeuroClock <= OutputNeuroNumber; OutputNeuroClock++)
			{
				ExampleERMS = ExampleERMS + pow((OutputNeuro[OutputNeuroClock]-TrueAnswer[ExampleClock][OutputNeuroClock] ), 2);
			}
			ExampleERMS = sqrt(ExampleERMS / (OutputNeuroNumber));
			cout << "\t\t\n\nERMS for training Example " << ExampleClock + 1 << " = " << ExampleERMS << endl << endl;

		}
	}
}

