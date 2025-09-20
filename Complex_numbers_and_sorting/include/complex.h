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
    Complex sum(const Complex& other) const;
    Complex diff(const Complex& other) const;
    double abs() const;
    Complex multiply(double val) const;
    void print() const;    
};

#endif //COMPLEX_H

