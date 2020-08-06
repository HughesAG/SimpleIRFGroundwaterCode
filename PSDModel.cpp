
#include <iostream>
#include <fstream>
using namespace std;

#include "PSDModel.h"
#include "GWNode.h"
#include "GWGrid.h"
#include "River.h"
#include "DEM.h"

int main() {


	// Call IRF methods

	initialise_model();
	run_model();
	finalise_model();

	return 0; 
}
void initialise_model() {

	// Open input files
	GridTestInfo.open("GridTest.in");
	GridTestRiverInfo.open("GridTestRiver.in");
	GridTestDEM.open("GridTestDEM.in");
	GridTestMain.open("GridTestMain.in");
	GridTestMain >> initialTime;
	GridTestMain >> timeStep;
	GridTestMain >> maxTime;
	GridTestHydro.open("GridTestHydro.in");
	// Open output file for heads
	GridTestOutHeads.open("GridTestHeads.out");
	// Open output file for flows
	GridTestOutFlows.open("GridTestFlows.out");
	// Open output file for hydros
	GridTestOutHydro.open("GridTestHydo.out");
	// set up rivers and DEM
	TestGridRiver = new River(GridTestRiverInfo);
	TestDEM = new DEM(GridTestDEM);
	// Setup grid of nodes
	TestGrid = new GWGrid(GridTestInfo, GridTestOutHeads, TestGridRiver, GridTestHydro);
	cout << "\n" << "Finished constructing grid" << "\n";
	return;
}
void run_model() {
	// run for n timesteps & write output
	for (t = initialTime; t <= maxTime; t = t + timeStep) {
		cout << "*** Change in heads for time " << t << " ***" << "\n";
		numTimeSteps++;
		do_timestep();
	}
	return;
}
void finalise_model() {
	// *** close files
	// input files
	GridTestInfo.close();
	GridTestRiverInfo.close();
	GridTestDEM.close();
	GridTestMain.close();
	GridTestHydro.close();
	// output files
	GridTestOutHeads.close();
	GridTestOutFlows.close();
	GridTestOutHydro.close();

	return;
}
// Do a single timesetp
void do_timestep() {
	// **** do a single timestep
	TestGrid->UpdateGWHeads(t, timeStep, numTimeSteps, GridTestOutHeads, GridTestOutFlows, TestGridRiver, TestDEM, GridTestOutHydro);
	return;
}
// Get & Set GWHead & recharge on a nodal basis
double getGWHead(int valuei, int valuej) {
	return TestGrid->getGWHead(valuei, valuej);
}
double getRecharge(int valuei, int valuej) {
	return TestGrid->getRecharge(valuei, valuej);
}
void setGWHead(double valueGWHead, int valuei, int valuej) {
	TestGrid->setGWHead(valueGWHead, valuei, valuej);
	return;
}
void setRecharge(double valueRecharge, int valuei, int valuej) {
	TestGrid->setRecharge(valueRecharge, valuei, valuej);
	return;
}

