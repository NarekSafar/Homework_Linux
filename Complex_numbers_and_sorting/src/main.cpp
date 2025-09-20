#include <iostream>
#include "complex.h"
#include "sort.h"
#include <vector>

int main() {
    Complex z_1(1.0, 2.0), z_2(0.1, 3.0), z_3(0, -7.9), z_4(3.43, -1.0),
	    z_5(1.0, 1.0), z_6(-12.7, 9.7), z_7(2.345, 0), z_8(-4.4, -8.02);

    Complex z_9 = z_2.sum(z_4);
    Complex z_10 = z_6.diff(z_7);
    Complex z_11 = z_3.multiply(2.5);

    std::vector<Complex> vec = {z_1, z_2, z_3, z_4, z_5, z_6, z_7, z_8, z_9, z_10, z_11};

    std::cout << "\nBefore sorting:\n";
    for(Complex num : vec) {
        num.print();
        std::cout << "\n";
    }

    bubbleSort(vec);

    std::cout << "\nSorted array of complex numbers:\n";
    for(Complex num : vec) {
        num.print();
	std::cout << "\n";
    }

    return 0;
}

