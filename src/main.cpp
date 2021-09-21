#include <iostream>
#include <SFML/Graphics.hpp>
#include "FluidBox.h"

using namespace std;

int main() 
{
    VD2 a = {{16, 3}, {7, -11}};
    VD1 b = {11, 13};
    VD1 x = {1, 1};

    VD1 y = FluidBox::gauss_seidel(a, b, x);
    cout << "x(" << x[0] << ", " << x[1] << ")" << endl;
    cout << "y(" << y[0] << ", " << y[1] << ")" << endl;

    cout << "Hello, World!" << endl;
    return 0;
}

