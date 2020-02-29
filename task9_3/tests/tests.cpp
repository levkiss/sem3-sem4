#include <iostream>
#include "../include/base.h"
#include "../include/rectangle.h"
#include "../include/circle.h"
#include "../include/cut.h"
#include "string"

int main() {
    std::cout << "Test 1: Circle" << std::endl;
    Circle cir(11, 2.8);
    std::cout << "True" << std::endl;

    std::cout << "Test 2: Rectangle" << std::endl;
    Rectangle rec(4);
    std::cout << "True" << std::endl;

    std::cout << "Test 3: Cutted" << std::endl;
    Cut cu(5, 3);
    std::cout << cu.calculateArea() << std::endl;

    std::cout << "Testing completed." << std::endl;
    return 0;
}
