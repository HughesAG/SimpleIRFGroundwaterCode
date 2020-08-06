#pragma once
class River
{
public:
	River(ifstream&);
	virtual ~River();
	double River::getXpos(int);
	double River::getYpos(int);
	double River::getElev(int);
private:
	int NumRiverNodes = 0, RivNodeID = 0;
	double *arrayRivNodeX;
	double *arrayRivNodeY;
	double *arrayRivElevation;
};

