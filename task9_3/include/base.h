#pragma once

#include <iostream>

using namespace::std;

class Base
{
public:
	Base();	// Constructor
	Base(double ar); // Parameter Constructor
	~Base(); // Destructor

	// Setter
	void setArea(double ar);

	// Getter
	double getArea() const;

	// New Method
	virtual double calculateArea() = 0;


private:

	double area;

};

