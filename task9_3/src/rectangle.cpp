#include <iostream>
#include "../include/base.h"
#include "../include/rectangle.h"


using namespace::std;

// Constructors
Rectangle::Rectangle()
{
	setEdge(0.0);
}

// Parameter Constructor
Rectangle::Rectangle(double ed) : Base()
{
	setEdge(ed);
}

// Destructor
Rectangle::~Rectangle()
{
	//cout << "Destroying rectangle..." << endl;
}

// Setters
void Rectangle::setEdge(double ed)
{
	if (ed < 0) {
		cerr << "The edge can not be a negative number.";
		exit(1);
	}
	else
		edge = ed;
}


// Getters
double Rectangle::getEdge() const
{
	return edge;
}


// Method
double Rectangle::calculateArea()
{
	double area;
	area = edge* edge;
	return area;
}
