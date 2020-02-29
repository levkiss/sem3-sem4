#include <iostream>
#include "../include/base.h"
#include "../include/circle.h"
#include "../include/cut.h"

using namespace::std;

// Constructors
cut::cut() : Rectangle(), Circle()
{
	setEdge(0);
	setRad(0);
}

// Parameter Constructor
cut::cut(double Edd, double aRadius) : Rectangle(), Circle()
{
	setEdge(Edd);
	setRad(aRadius);
}

// Destructor
cut::~cut()
{
	//cout << "Destroying cut..." << endl;
}

// Method
double cut::calculateArea()
{
	double area;
	area = Rectangle::edge * Rectangle::edge - 3.14159 * Circle::radius * Circle::radius;
	return area;
}
