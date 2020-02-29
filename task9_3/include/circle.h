#pragma once

#include <iostream>
#include "base.h"

using namespace::std;

class Circle : public Base // <- Main Class
{
public:

	// Constructors
	Circle();
	Circle(double aRadius);
	~Circle();

	// Setters
	void setRad(double aRadius);

	// Getters
	double getRad() const;

	// Method
	double calculateArea() override;


protected:
	double radius;
};


