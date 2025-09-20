#include <iostream>
#include "sort.h"

void bubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for(int i = 0; i < n - 1; ++i) {
        for(int j = 0; j < n - i -1; ++j) {
	    if(vec[j].abs() > vec[j + 1].abs()) {
	        std::swap(vec[j], vec[j + 1]);
	    }
	}
    }
}

