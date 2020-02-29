#pragma once

#include <iostream>
#include "base.h"
#include "rectangle.h"
#include "circle.h"

using namespace::std;

class cut : public Rectangle, public Circle	// <- Main Class
{
public:
	cut();	// Constructor
	cut(double Edd, double aRadius);  // Parameter Constructor
	~cut();	// Destructor

	// Methods
	double calculateArea() override;
};