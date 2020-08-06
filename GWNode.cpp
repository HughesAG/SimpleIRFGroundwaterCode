#include<iostream>
#include<math.h>
#include<algorithm>
using namespace std;
#include "GWNode.h"
#include "GWGrid.h"
#include "River.h"

// Default constructor

GWNode::GWNode(int RivID, double initialGWhead, double satdThickness, double valueRecharge)
{

	RiverNodeID = RivID;
	GWhead = initialGWhead;
	previousGWhead = GWhead;
	saturatedThickness = satdThickness; 
	Recharge = valueRecharge;

}

GWNode::GWNode(double X, double Y)
{
	Xpos = X;
	Ypos = Y;
}

void GWNode::setXpos(double X){
Xpos = X;
return;
}

void GWNode::setYpos(double Y){
Ypos = Y;
return;
}
// PSD2: methods to set nearest neighbours
void GWNode::setEastNode(GWNode *gwNode) {
	eastNode = gwNode;
	return;
}
// PSD2: methods to set nearest neighbours
void GWNode::setWestNode(GWNode *gwNode) {
	westNode = gwNode;
	return;
}
// PSD2: methods to set nearest neighbours
void GWNode::setNorthNode(GWNode *gwNode) {
	northNode = gwNode;
	return;
}
// PSD2: methods to set nearest neighbours
void GWNode::setSouthNode(GWNode *gwNode) {
	southNode = gwNode;
	return;
}
// PSD2: method to update GW head
void GWNode::updateGWhead() {
	previousGWhead = GWhead;
	return;
}
// PSD2: method to update flow
void GWNode::storeFlowToFromSurface(double flow) {
	flowToFromSurface = flow;
	return;
}
// PSD2: method to update flow
void GWNode::setGWHead(double head) {
	GWhead = head;
	return;
}


double GWNode::getXpos(){
return Xpos;
}

double GWNode::getYpos(){
return Ypos;
}
// PSD2: method to get current GW head
double GWNode::getGWHead(){
	return GWhead;
}
// PSD2: method to get previous GW head
double GWNode::getPreviousGWHead(){
	return previousGWhead;
}

double GWNode::getRecharge() {
	return Recharge;
}

void GWNode::setRecharge(double valueRecharge) {
	Recharge = valueRecharge;
	return;
}

double GWNode::getGWHeadChange(double deltaT, double HorizontalK, double SpecificStorage, double valueRecharge, double deltaX, double deltaY){

	// calculate GW head for any given time
	// setting up variables

	double westGWhead = 0, eastGWhead = 0, northGWhead = 0, southGWhead = 0;
	double areaFactorEW = 1.0;
	double areaFactorNS = 1.0;
	// connect nodes and dela with boundaries
	if (westNode != NULL) { 
		westGWhead = westNode->getPreviousGWHead(); 
	} else {
		westGWhead = eastNode->getPreviousGWHead();
		areaFactorEW = 0.5;
	}
	if (eastNode != NULL) { 
		eastGWhead = eastNode->getPreviousGWHead();
	} else {
		eastGWhead = westNode->getPreviousGWHead();
		areaFactorEW = 0.5;
	}
	if (northNode != NULL) { 
		northGWhead = northNode->getPreviousGWHead();
	} else {
		northGWhead = southNode->getPreviousGWHead();
		areaFactorNS = 0.5;
	}
	if (southNode != NULL) { 
		southGWhead = southNode->getPreviousGWHead();
	} else {
		southGWhead = northNode->getPreviousGWHead();
		areaFactorNS = 0.5;
	}
	// Calculate new head at the node
	double mainCoefficient = (4 * HorizontalK * saturatedThickness * deltaT) / (SpecificStorage * saturatedThickness * deltaX * deltaY);
	if ((1 - mainCoefficient) < 0) { cout << " WARNING Explicit criteria has been violated: " << mainCoefficient << "\n"; }
	// cout << " Node at " << " Xloc " << getXpos() << " Yloc " << getYpos();
	// cout << " Head to N is " << northGWhead << " Head to E is " << eastGWhead << " Head to S is " << southGWhead << " Head to W is " << westGWhead << "\n";
	surroundingGWheads = ( westGWhead + eastGWhead + northGWhead + southGWhead) / 4;
	flowTerm = ( flowToFromSurface + Recharge) * deltaT / (saturatedThickness * SpecificStorage);
	// flowTerm = 0.001 * deltaT / (saturatedThickness * SpecificStorage); // Test for Analytical solution
	GWhead = (1 - mainCoefficient) * previousGWhead + mainCoefficient * surroundingGWheads + flowTerm;
	// cout << " Calc h " << GWhead << " prev h " << previousGWhead << " for s'ding H of " << surroundingGWheads << " q " << flowTerm << "\n";
	return GWhead;
}

GWNode::~GWNode()
{
}
