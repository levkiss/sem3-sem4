#include <iostream>
#include "../include/circle.h"
#include "../include/base.h"

using namespace::std;

// Constructor
Circle::Circle()
{
	setRad(0.0);
}

// Parameter Constructor
Circle::Circle(double aRadius) : Base()
{
	setRad(aRadius);
}

// Destructor
Circle::~Circle()
{
	//cout << "Destroying circle..." << endl;
}

// Setters
void Circle::setRad(double aRadius)
{
	if (aRadius < 0) {
		cerr << "The radius can not be a negative number.";
		exit(1);
	}
	else
		radius = aRadius;
}

// Getters
double Circle::getRad() const
{
	return radius;
}

// Method
double Circle::calculateArea()
{
	double area;
	area = 3.14159 * radius * radius;
	return area;
}
