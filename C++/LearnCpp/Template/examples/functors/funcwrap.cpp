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
#include <iostream>
#include <cstdlib>

// wrapper for function pointers to function objects
template<int (*FP)()>
class FunctionReturningIntWrapper {
  public:
    int operator() () {
        return FP();
    }
};

// example function to wrap
int random_int()
{
    return std::rand();  // call standard C function
}

// client that uses function object type as template parameter
template <typename FO>
void initialize (std::vector<int>& coll)
{
    FO fo;  // create function object
    for (std::vector<int>::size_type i=0; i<coll.size(); ++i) {
        coll[i] = fo();  // call function for function object
    }
}

int main()
{
    // create vector with 10 elements
    std::vector<int> v(10);

    // (re)initialize values with wrapped function
    initialize<FunctionReturningIntWrapper<random_int> >(v);

    // output elements
    for (std::vector<int>::size_type i=0; i<v.size(); ++i) {
        std::cout << "coll[" << i << "]: " << v[i] << std::endl;
    }
}
