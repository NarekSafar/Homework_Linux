#include <iostream>
#include "complex.h"
#include <cmath>

Complex::Complex(double a, double b) : a(a), b(b) {};
double Complex::getReal() const { return a; };
double Complex::getImag() const { return b; };
Complex Complex::operator+(const Complex& other) const {
    return Complex(a + other.a, b + other.b);
}
Complex Complex::operator-(const Complex& other) const {
    return Complex(a - other.a, b - other.b);
}
double Complex::abs() const {
    return sqrt(a*a + b*b);
}			
Complex Complex::operator*(double val) const {
    return Complex(val * a, val * b);
}
bool Complex::operator>(const Complex& other) const {
    return this->abs() > other.abs(); 
}
std::ostream& operator<<(std::ostream& os, const Complex& other) {
     if (other.a == 0 && other.b == 0) { os << "0";}

        if (other.a != 0) os << other.a;

        if (other.b != 0) {
            if (other.b > 0 && other.a != 0) os << " + ";
            else if (other.b < 0 && other.a != 0) os << " - ";

            if (std::abs(other.b) != 1) os << std::abs(other.b);
            os << "i";
	}
	return os;
}
        
