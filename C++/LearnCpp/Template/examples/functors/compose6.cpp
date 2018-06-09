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
#include <iostream>
#include "funcptr.hpp"
#include "compose6.hpp"
#include "composeconv.hpp"

double add(double a, double b) 
{
    return a+b;
}

double twice(double a) 
{
    return 2*a;
}

int main() 
{
    std::cout << "compute (20+7)*2: "
              << compose(func_ptr(add),func_ptr(twice))(20,7)
              << '\n';
}
