#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex {
private:
    double a;
    double b;
public:
    Complex(double a = 0.0, double b = 0.0);
    double getReal() const;
    double getImag() const;
    double abs() const;
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(double val) const;
    bool operator>(const Complex& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};

#endif //COMPLEX_H

