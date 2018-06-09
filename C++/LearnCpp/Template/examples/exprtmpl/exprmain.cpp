/* The following code example is taken from the book
 * "C++ Templates - The Complete Guide"
 * by David Vandevoorde and Nicolai M. Josuttis, Addison-Wesley, 2002
 *
 * (C) Copyright David Vandevoorde and Nicolai M. Josuttis 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include "exprops1.hpp"
#include "exprscalar.hpp"
#include "exprarray.hpp"
#include "exprops2.hpp"
#include <iostream>

template <typename T>
void print (T const& c)
{
    for (int i=0; i<8; ++i) {
        std::cout << c[i] << ' ';
    }
    std::cout << "..." << std::endl;
}

int main()
{
    Array<double> x(1000), y(1000);

    // initialize arrays with some values
    for (int i=0; i<1000; ++i) {
        x[i] = i;
        y[i] = x[i]+x[i];
    }
    //...
    
    std::cout << "x: ";
    print(x);

    std::cout << "y: ";
    print(y);

    x = 1.2 * x;
    std::cout << "x = 1.2 * x: ";
    print(x);

    x = 1.2*x + x*y;
    std::cout << "1.2*x + x*y: ";
    print(x);

    x = y;
    std::cout << "after x = y: ";
    print(x);
}
