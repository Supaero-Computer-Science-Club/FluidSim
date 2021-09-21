#include <iostream>
#include <SFML/Graphics.hpp>
#include "FluidBox.h"

using namespace std;

int main() 
{
    // See https://en.wikipedia.org/wiki/Gauss%E2%80%93Seidel_method#An_example_for_the_matrix_version
    // This is the resolution of this system
    VD2 a = {{16, 3}, {7, -11}};
    VD1 b = {11, 13};
    VD1 x = {1, 1};

    VD1 y = FluidBox::gauss_seidel(a, b, x);
    cout << "i=1\nx(" << x[0] << ", " << x[1] << ")" << endl;
    cout << "y(" << y[0] << ", " << y[1] << ")" << endl << endl;

    x = y;
    y = FluidBox::gauss_seidel(a, b, x);
    cout << "i=2\nx(" << x[0] << ", " << x[1] << ")" << endl;
    cout << "y(" << y[0] << ", " << y[1] << ")" << endl << endl;

    for (int i=3; i<8; i++) {
        x = y;
        y = FluidBox::gauss_seidel(a, b, x);
    }

    cout << "i=7\nx(" << x[0] << ", " << x[1] << ")" << endl;
    cout << "y(" << y[0] << ", " << y[1] << ")" << endl;


    cout << "Hello, World!" << endl;
    return 0;
}

