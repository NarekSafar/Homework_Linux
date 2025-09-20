#include <iostream>
#include "complex.h"
#include <cmath>

Complex::Complex(double a, double b) : a(a), b(b) {};
double Complex::getReal() const { return a; };
double Complex::getImag() const { return b; };
Complex Complex::sum(const Complex& other) const {
    return Complex(a + other.a, b + other.b);
}
Complex Complex::diff(const Complex& other) const {
    return Complex(a - other.a, b - other.b);
}
double Complex::abs() const {
    return sqrt(a*a + b*b);
}			
Complex Complex::multiply(double val) const {
    return Complex(val * a, val * b);
}
void Complex::print() const {
     if (a == 0 && b == 0) { std::cout << "0"; return; }

        if (a != 0) std::cout << a;

        if (b != 0) {
            if (b > 0 && a != 0) std::cout << " + ";
            else if (b < 0 && a != 0) std::cout << " - ";

            if (std::abs(b) != 1) std::cout << std::abs(b);
            std::cout << "i";
        }
}
        
