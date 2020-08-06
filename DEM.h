#pragma once
class DEM
{
public:
	DEM(ifstream&);

	double DEM::getDEMValue(int, int);
	bool DEM::getDEMSwitch();

	virtual ~DEM();
private:

	int maxXnodes = 5, maxYnodes = 5;
	double DeltaX = 1, DeltaY = 1;
	double XOffset = 0, YOffset = 0;
	float **arrayDEM;
	bool DEMSwitch = true;
};

