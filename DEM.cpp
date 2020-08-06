
#include<iostream>
#include <fstream>
using namespace std;


#include "DEM.h"

DEM::DEM(ifstream& InputFile)
{

	InputFile >> DEMSwitch;
	if (DEMSwitch) {
		cout << "*** Groundwater flooding calculated" << "\n";
	}
	else {
		cout << "*** Groundwater flooding NOT calculated" << "\n";
	}
	InputFile >> maxXnodes >> maxYnodes;
	cout << "X Nodes: " << maxXnodes << " Y Nodes: " << maxYnodes << "\n";
	InputFile >> DeltaX >> DeltaY;
	cout << " DeltaX: " << DeltaX << " DeltaY: " << DeltaY << "\n";
	InputFile >> XOffset >> YOffset;
	cout << " XOffset: " << XOffset << " YOffset: " << YOffset << "\n";
	cout << "\n";

	// set up arrayDEM dynamically
	arrayDEM = new float *[maxXnodes + 1];
	for (int i = 0; i <= maxXnodes; i++) {
		arrayDEM[i] = new float[maxYnodes + 1];
	}
	// Read in DEM node by node
	for (int j = 1; j <= maxYnodes; j++) {
		for (int i = 1; i <= maxXnodes; i++) {
			InputFile >> arrayDEM[i][j];
		}
	}
	
	for (int j = 1; j <= maxYnodes; j++) {
		cout << "DEM: Row - " << j << " ";
		for (int i = 1; i <= maxXnodes; i++) {
			cout << arrayDEM[i][j] << " ";
		}
		cout << "\n" ;
	}

}

double DEM::getDEMValue(int i, int j) {
	return arrayDEM[i][j];
}

bool DEM::getDEMSwitch() {
	return DEMSwitch;
}

DEM::~DEM()
{
}
