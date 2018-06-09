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
#include "holder2.hpp"
#include "trule.hpp"

class Something {
};

void read_something (Something* x)
{
}

Trule<Something> load_something() 
{
    Holder<Something> result(new Something);
    read_something(result.get());
    return result;
}

int main() 
{
    Holder<Something> ptr(load_something());
    //...
}
