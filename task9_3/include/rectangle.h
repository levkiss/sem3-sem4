#pragma once

#include <iostream>
#include "base.h"

using namespace::std;

class Rectangle : public Base		// <- Main Class
{
public:
	Rectangle();	// Constructor
	Rectangle(double ed);  // Parameter Constructor
	~Rectangle();	// Destructor

	// Setters
	void setEdge(double ed);

	// Getters
	double getEdge() const;

	// Methods
	double calculateArea() override;

protected:
	double edge;
};

