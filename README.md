# SimpleIRFGroundwaterCode
Very simple groundwater model code that solves the 2D groundwater flow equation explicitly. It has an "Initialise, Run &amp; Finalise" structure to aid model integration

Structure of code is as follows:

Main method is PSDModel.cpp

Digitial Evelation Model for simple groundwater-surafce interaction is held here: DEM.cpp
Simple river modle (as fixed heads) is held here: River.cpp
The model grids consists of a node class (GWNode.cpp) held in a grid class (GWGrid.cpp)
