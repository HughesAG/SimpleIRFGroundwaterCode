#pragma once
class GWNode
{

public:
	GWNode(int, double, double, double);
	GWNode(double, double);
	~GWNode();
	void setXpos(double);
	void setYpos(double);
	void setEastNode(class GWNode *);
	void setWestNode(class GWNode *);
	void setNorthNode(class GWNode *);
	void setSouthNode(class GWNode *);
	void updateGWhead();
	void storeFlowToFromSurface(double);
	void setGWHead(double);
	double getXpos();
	double getYpos();
	double getGWHead();
	double getPreviousGWHead();
	double getRecharge();
	void setRecharge(double);
	double getGWHeadChange(double, double, double, double, double, double);
private:
	double calcFactorial(int);
	double calcSfw(int, int);
	double Xpos = 0, Ypos = 0;
	double GWhead = 0, previousGWhead = 0;
	double saturatedThickness = 0, Recharge = 0.;
	double flowToFromSurface = 0;
	double flowTerm = 0, surroundingGWheads = 0;
	int RiverNodeID = 0;
	double DSC = 0; // x0 half width of river
	double pLTV = 0, pLTVss = 0;
	double timeDss;
	double sBarD, sBarDcum;
	// double GWHead = 0;
	double x = 0, xD = 0;
	double xDiff = 0, yDiff = 0;
	GWNode *eastNode, *westNode, *northNode, *southNode;
	// define variables for controlling Stehfest weights
	const int maxNumSfw = 18;
	double *arraySfw;
};

