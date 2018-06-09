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
#include <vector>
#include <list>
#include <stack>
#include <iostream>
#include <typeinfo>

template <typename T> 
class ElementT;                    // primary template

template <typename T>
class ElementT<std::vector<T> > {  // partial specialization
  public:
    typedef T Type;
};

template <typename T>
class ElementT<std::list<T> > {    // partial specialization
  public:
    typedef T Type;
};

template <typename T>
class ElementT<std::stack<T> > {   // partial specialization
  public:
    typedef T Type;
};

template <typename T>
void print_element_type (T const & c)
{
    std::cout << "Container of "
              << typeid(typename ElementT<T>::Type).name()
              << " elements.\n";
}

int main()
{
    std::stack<bool> s;
    print_element_type(s);
}
