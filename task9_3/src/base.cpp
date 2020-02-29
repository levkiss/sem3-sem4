#include <iostream>
#include "../include/base.h"

using namespace::std;

// Constructor
Base::Base()
{
	setArea(0.0);
}

// Parameter Constructor
Base::Base(double ar)
{
	setArea(ar);
}

// Destructor
Base::~Base()
{
	// cout << "Destroying main object..." << endl;
}

// Setters
void Base::setArea(double ar)
{
	if (ar < 0){
		cout << "The area can not be a negative number.";
		exit(1);
	}
	else{
		area = ar;
	}
}
// Getters
double Base::getArea() const
{
	return area;
}

