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
#include <string>
#include <iostream>

template<typename T> 
std::string f(T) 
{ 
    return "Template"; 
}

std::string f(int&) 
{ 
    return "Nontemplate"; 
}

int main() 
{
    int x = 7;
    std::cout << f(x) << std::endl;
}
