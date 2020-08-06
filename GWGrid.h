#pragma once
class GWGrid
{
public:
	GWGrid(ifstream&, ofstream&, class River *, ifstream&);
	virtual ~GWGrid();

	void UpdateGWHeads(double, double, int,  ofstream&, ofstream&, class River *, class DEM *, ofstream&);
	float getHydraulicConductivity();
	float getSpecificStorage();
	double getRecharge(int, int);
	double getRecharge();
	void setRecharge(double);
	void setRecharge(double, int, int);
	double getGWHead(int, int);
	void setGWHead(double, int, int);

private:
	class GWNode ***arrayGWNode;
	GWNode *easternNode, *westernNode, *northernNode, *southernNode;
	int maxXnodes = 5, maxYnodes = 5;
	double DeltaX = 1, DeltaY = 1;
	double XOffset = 0, YOffset = 0;
	double Kh = 1, Ss = 0.001; // Hydraulic cond. & Specific storage
	double SaturatedThickness = 10;
	double InitialH = 0, ChangeInStage = 1.0, flowLandSurface = 0 ;
	double Recharge = 0;
	double DistanceStreamCentre = 10; // x0 half width of river
	int dummy, NumStage;
	double cumHeadChange, timeForHeads;
	double *arrayStage;
	int **arrayRiverID; 
	int numHydros = 1;
	int *rowHydro, *colHydro;
};

