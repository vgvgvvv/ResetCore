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
#include <string>
#include <typeinfo>
#include "funcptr.hpp"

double seven() 
{
    return 7.0;
}

std::string more() 
{
    return std::string("more");
}

template <typename FunctorT>
void demo (FunctorT func) 
{
    std::cout << "Functor returns type "
              << typeid(typename FunctorT::ReturnT).name() << '\n'
              << "Functor returns value "
              << func() << '\n';
}

int main() 
{
    demo(func_ptr(seven));
    demo(func_ptr(more));
}
