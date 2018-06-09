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
#include "math1.hpp"
#include "compose4.hpp"
#include "composeconv.hpp"

template<typename FO>
void print_values (FO fo) 
{
    for (int i=-2; i<3; ++i) {
        std::cout << "f(" << i*0.1
                  << ") = " << fo(i*0.1)
                  << "\n";
    }
}

int main()
{
    // print sin(abs(-0.5))
    std::cout << compose(Abs(),Sine())(0.5) << "\n\n";

    // print abs() of some values
    print_values(Abs());
    std::cout << '\n';

    // print sin() of some values
    print_values(Sine());
    std::cout << '\n';

    // print sin(abs()) of some values
    print_values(compose(Abs(),Sine()));
    std::cout << '\n';

    // print abs(sin()) of some values
    print_values(compose(Sine(),Abs()));
    std::cout << '\n';

    // print sin(sin()) of some values
    print_values(compose(Sine(),Sine()));
}
