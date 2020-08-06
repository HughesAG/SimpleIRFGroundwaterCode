#pragma once

// Define variables for the model

double initialTime = 0.1, timeStep = 0.1, maxTime = 1.0, t = 0;
int numTimeSteps = 0;

// Define input and output streams for file reading, etc

ifstream GridTestInfo;
ifstream GridTestRiverInfo;
ifstream GridTestDEM;
ifstream GridTestMain;
ifstream GridTestHydro;
ofstream GridTestOutHeads;
ofstream GridTestOutFlows;
ofstream GridTestOutHydro;

// define methods within the main programme
// Main IRF ones
void initialise_model();
void run_model();
void finalise_model();
// Supporting methods
void do_timestep();
double getGWHead(int, int);
double getRecharge(int, int);
void setGWHead(double, int, int);
void setRecharge(double, int, int);

// Define objects instantiated for model

class River;
River *TestGridRiver;
class DEM;
DEM *TestDEM;
class GWGrid;
GWGrid *TestGrid;
