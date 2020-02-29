#include <iostream>
#include "matrix.h"

int main() {
    std::cout << "try matrix x(5,4) and print it" << std::endl;
    try {
        matrix m(5,4);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x(0,4) and print it" << std::endl;
    try {
        matrix m(0,4);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x(-1,4) and print it" << std::endl;
    try {
        matrix m(-1,4);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x(123) and print it" << std::endl;
    try {
        matrix m(123);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({1,2,3,4,5},4) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m(buf, 4);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({1,2,3,4,5},5) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m(buf, 5);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({1,2,3,4,5},6) and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m(buf, 6);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x(5, {1,2,3,4,5}); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix x(5, buf);
        std::cout << x << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({{1,2,3,4,5}}) and print it" << std::endl;
    try {
        char buf[14] = "{{1,2,3,4,5}}";
        matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({{1},{2},{3},{4},{5}}) and print it" << std::endl;
    try {
        char buf[22] = "{{1},{2},{3},{4},{5}}";
        matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({{1},{2},{3},{4},{5, 6}}) and print it" << std::endl;
    try {
        char buf[25] = "{{1},{2},{3},{4},{5, 6}}";
        matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({{1,2,3},{4,5,6},{7,8,9}}) and print it" << std::endl;
    try {
        char buf[26] = "{{1,2,3},{4,5,6},{7,8,9}}";
        matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x({{1,2,3},{lalala},{7,8,9}}) and print it" << std::endl;
    try {
        char buf[27] = "{{1,2,3},{lalala},{7,8,9}}";
        matrix m(buf);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::identity(5); and print it" << std::endl;
    try {
        matrix m = matrix::identity(5);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::identity(0); and print it" << std::endl;
    try {
        matrix m = matrix::identity(0);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::diagonal({1,2,3,4,5},4); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m = matrix::diagonal(buf, 4);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::diagonal({1,2,3,4,5},5); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m = matrix::diagonal(buf, 5);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::diagonal({1,2,3,4,5},6); and print it" << std::endl;
    try {
        double buf[5] = {1,2,3,4,5};
        matrix m = matrix::diagonal(buf, 6);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = matrix::diagonal(NULL,100); and print it" << std::endl;
    try {
        double* buf = NULL;
        matrix m = matrix::diagonal(buf, 100);
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "now we are using matrix \"x({{1,2,3},{4,5,6}})\":" << std::endl;
    char buf[18] = "{{1,2,3},{4,5,6}}";
    matrix x(buf);
    std::cout << "try x.rows(); and print it" << std::endl;
    try {
        int i = x.rows();
        std::cout << i << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try x.columns(); and print it" << std::endl;
    try {
        int i = x.columns();
        std::cout << i << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try x.set(1,2,10); and print it" << std::endl;
    try {
        x.set(1,2,10);
        std::cout << 10 << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try x.set(100,-1,10); and print it" << std::endl;
    try {
        x.set(100,-1,10);
        std::cout << x << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = x[0]; and print it" << std::endl;
    try {
        matrix m = x[0];
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = x[1]; and print it" << std::endl;
    try {
        matrix m = x[1];
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = x[2]; and print it" << std::endl;
    try {
        matrix m = x[2];
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = x[3]; and print it" << std::endl;
    try {
        matrix m = x[3];
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix m = x * 5; and print it" << std::endl;
    try {
        matrix m = x * 5;
        std::cout << m << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "try matrix x *= 6; and print it" << std::endl;
    try {
        x *= 6;
        std::cout << x << std::endl;
    }
    catch (error_catcher error) {
        std::cerr << error.what() << std::endl;
    }
    std::cout << "Testing completed." << std::endl;
    return 0;
}