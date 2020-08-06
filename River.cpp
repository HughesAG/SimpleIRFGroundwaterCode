#include<iostream>
#include <fstream>
using namespace std;

#include "River.h"


River::River(ifstream& InputFile)
{
	double dummyX, dummyY, dummyElev;
	InputFile >> NumRiverNodes;
	arrayRivNodeX = new double [NumRiverNodes + 1];
	arrayRivNodeY = new double [NumRiverNodes + 1];
	arrayRivElevation = new double[NumRiverNodes + 1];
	for (int i=1; i <= NumRiverNodes; i++){
		InputFile >> RivNodeID >> dummyX >> dummyY >> dummyElev;
		arrayRivNodeX[RivNodeID] = dummyX;
		arrayRivNodeY[RivNodeID] = dummyY;
		arrayRivElevation[RivNodeID] = dummyElev;
		// cout << "River Node ID is: " << RivNodeID << " at X: " << arrayRivNodeX[RivNodeID] << " at Y: " << arrayRivNodeY[RivNodeID] << "\n";
	}
}

double River::getXpos(int RivID){
	// Return X location of River Node
	return arrayRivNodeX[RivID];
}

double River::getYpos(int RivID){
	// Return Y location of River Node
	return arrayRivNodeY[RivID];
}

double River::getElev(int RivID) {
	// return River Elevation
	return arrayRivElevation[RivID];
}

River::~River()
{
}
