#include <iostream>
#include <fstream>
using namespace std;

#include "GWGrid.h"
#include "GWNode.h"
#include "DEM.h"
#include "River.h"


GWGrid::GWGrid(ifstream& InputFile, ofstream& OutputFile, class River *TestGridRiver, ifstream& HydroInputFile)
{
	cout << "Constructor for Grids of GWNodes" << "\n";
	// Read in data fromm file
	InputFile >> Kh;
	OutputFile << "Hydraulic conductivity is: " << Kh << "\n";
	InputFile >> Ss;
	OutputFile << "Specific Storage is: " << Ss << "\n";
	InputFile >> SaturatedThickness;
	OutputFile << "Saturated Thickness is: " << SaturatedThickness << "\n";
	InputFile >> Recharge;
	OutputFile << "Recharge is: " << Recharge << "\n";
	InputFile >> InitialH;
	OutputFile << " Initial Head: " << InitialH << "\n";
	InputFile >> NumStage;
	OutputFile << " Number of time periods for CiSt: " << NumStage << "\n";
	arrayStage = new double[NumStage + 1];
	for (int i = 0; i <= NumStage; i++){
		InputFile >> dummy >> arrayStage[i];
		OutputFile << " Change in stage: " << i << " is " << arrayStage[i] << "\n";
	}
	
	InputFile >> maxXnodes >> maxYnodes;
	OutputFile << "X Nodes: " << maxXnodes << " Y Nodes: " << maxYnodes << "\n";
	InputFile >> DeltaX >> DeltaY;
	OutputFile << " DeltaX: " << DeltaX << " DeltaY: " << DeltaY << "\n";
	InputFile >> XOffset >> YOffset;
	OutputFile << " XOffset: " << XOffset << " YOffset: " << YOffset << "\n";
	OutputFile << "\n";
	// read in hydro info
	HydroInputFile >> numHydros;
	OutputFile << " Number of hydros are: " << numHydros << "\n";
	rowHydro = new int [numHydros + 1];
	colHydro = new int [numHydros + 1];
	for (int iCount = 1; iCount <= numHydros; iCount++) {
		OutputFile << " Reading Hydro no:" << iCount << "\n";
		HydroInputFile >> rowHydro[iCount] >> colHydro[iCount];
	}


	// set up arrayRiverID dynamically
	arrayRiverID = new int *[maxXnodes + 1];
	for (int i = 0; i <= maxXnodes; i++){
		arrayRiverID[i] = new int [maxYnodes + 1];
	}
	// Read in River Node IDs
	for (int j = 1; j <= maxYnodes; j++){
		for (int i = 1; i <= maxXnodes; i++){
			InputFile >> arrayRiverID[i][j];
		}
	}
	OutputFile << "\n River IDs" << "\n";
	for (int j = 1; j <= maxYnodes; j++){
		for (int i = 1; i <= maxXnodes; i++){
			OutputFile << arrayRiverID[i][j] << " ";
		}
		OutputFile << "\n";
	}
	// build array to contain grid of GWNodes
	// PSD2: Add extra row and column around edge to  cope with boundaries
	arrayGWNode = new GWNode **[maxXnodes + 2];
	for (int i = 0; i <= maxXnodes + 1; i++){
		arrayGWNode[i] = new GWNode *[maxYnodes + 2];
	}
	for (int j = 0; j <= maxYnodes + 1; j++){
		for (int i = 0; i <= maxXnodes + 1; i++){
			arrayGWNode[i][j] = NULL;
		}
	}
	cout << " Constructing grid of GWNodes" << "\n";
	for (int j = 1; j <= maxYnodes; j++){
		for (int i = 1; i <= maxXnodes; i++){
			if (arrayRiverID[i][j] >= 0) {
				double initialGWhead = 0;
				if (arrayRiverID[i][j] > 0) {
					// Temporary fixed head to represent river
					initialGWhead = TestGridRiver->getElev(arrayRiverID[i][j]);
					cout << " Elevation is " << initialGWhead << " ";
				}
				else {
					initialGWhead = InitialH;
				}
				arrayGWNode[i][j] = new GWNode(arrayRiverID[i][j], initialGWhead, SaturatedThickness, Recharge); 
			}
			
		}
	}
	// create grid of GWNode objects
	for (int j = 1; j <= maxYnodes; j++){
		for (int i = 1; i <= maxXnodes; i++){
			if (arrayGWNode[i][j] != NULL) {
					arrayGWNode[i][j]->setXpos(DeltaX*(i - 1) + DeltaX / 2 + XOffset);
					arrayGWNode[i][j]->setYpos((DeltaY*(maxYnodes - 1)) - DeltaY*(j - 1) + DeltaY / 2 + YOffset);
					// PSD2: setup nearest neighbours for each node
					easternNode = arrayGWNode[i + 1][j];
					westernNode = arrayGWNode[i - 1][j];
					northernNode = arrayGWNode[i][j - 1];
					southernNode = arrayGWNode[i][j + 1];
					if (easternNode != NULL) {
						arrayGWNode[i][j]->setEastNode(easternNode);
					}
					if (westernNode != NULL) {
						arrayGWNode[i][j]->setWestNode(westernNode);
					}
					if (northernNode != NULL) {
						arrayGWNode[i][j]->setNorthNode(northernNode);
					}
					if (southernNode != NULL) {
						arrayGWNode[i][j]->setSouthNode(southernNode);
					}
					// cout << " arrayGWnode " << arrayGWNode[i][j] << " Xloc " << arrayGWNode[i][j]->getXpos() << " Yloc " << arrayGWNode[i][j]->getYpos() << " for: i= " << i << " j is " << j << "\n";

			}
		}
	}
}

void GWGrid::UpdateGWHeads(double time, double deltaT, int numTS, ofstream& OutputHeadFile, ofstream& OutputFlowFile, class River *TestGridRiver, class DEM *TestDEM, ofstream& OutputHydroFile){
	// Write out GW heads
	OutputHeadFile << "Time is: " << time << "\n";
	OutputFlowFile << "Time is: " << time << "\n";
	int iStageCountMax = 0;
	float currentH, coeffLSExchange = 1e-5;
	double currentDEM;
	for (int j = 1; j <= maxYnodes; j++){
		for (int i = 1; i <= maxXnodes; i++){
			currentH = 0;
			// Check if node has been instantiated
			if (arrayRiverID[i][j] > 0) { // Check for river node
				currentH = arrayGWNode[i][j]->getGWHead();
				OutputHeadFile << currentH << " ";
				OutputFlowFile << 0 << " ";
			} else if(arrayRiverID[i][j] < 0){
				OutputHeadFile << arrayRiverID[i][j] << " ";
				OutputFlowFile << arrayRiverID[i][j] << " ";
			} else {
				// Calculate current head
				currentH = arrayGWNode[i][j]->getGWHeadChange(deltaT, Kh, Ss, Recharge, DeltaX, DeltaY);
				OutputHeadFile << currentH << " ";
				// Calculate current flow from head
				currentDEM = TestDEM->getDEMValue(i,j);
				if (currentH >= currentDEM) {
					// -ve flow to represent outflow from aquifer
					flowLandSurface = coeffLSExchange * (currentDEM - currentH);
				}else
				{
					flowLandSurface = 0;
				}
				arrayGWNode[i][j]->storeFlowToFromSurface(flowLandSurface);
				OutputFlowFile << flowLandSurface << " ";
			}

		}

		OutputHeadFile << "\n";
		OutputFlowFile << "\n";
	}
	OutputHydroFile << time << ",";
	for (int iCount = 1; iCount <= numHydros; iCount++) {
		OutputHydroFile << arrayGWNode[rowHydro[iCount]][colHydro[iCount]]->getGWHead() << ",";
	}
	OutputHydroFile << "\n";
	

	// PSD2: Update heads at end of timestep
	for (int j = 1; j <= maxYnodes; j++) {
		for (int i = 1; i <= maxXnodes; i++) {
			arrayGWNode[i][j]->updateGWhead();
		}
	}


}
float GWGrid::getHydraulicConductivity() {
	return Kh;
}
float GWGrid::getSpecificStorage() {
	return Ss;
}
double GWGrid::getRecharge() {
	return Recharge;
}
double GWGrid::getRecharge(int i, int j) {
	return arrayGWNode[i][j]->getRecharge();
}
void GWGrid::setRecharge(double valueRecharge) {
	Recharge = valueRecharge;
	return;
}
void GWGrid::setRecharge(double valueRecharge, int i, int j) {
	arrayGWNode[i][j]->setRecharge(valueRecharge);
	return;
}
double GWGrid::getGWHead(int i, int j) {
	return arrayGWNode[i][j]->getGWHead();
}
void GWGrid::setGWHead(double valueGWHead, int i, int j) {
	arrayGWNode[i][j]->setGWHead(valueGWHead);
	return;
}

GWGrid::~GWGrid()
{
}
