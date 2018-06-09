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
#include <cmath>
#include <cstdlib>

template <typename T>
class Abs {
  public:
    // to fit in the framework:
    enum { NumParams = 1 };
    typedef T ReturnT;
    typedef T Param1T;

    // ``function call'':
    T operator() (T v) const { 
        return std::abs(v); 
    }
};

template <typename T>
class Sine {
  public:
    // to fit in the framework:
    enum { NumParams = 1 };
    typedef T ReturnT;
    typedef T Param1T;

    // ``function call'':
    T operator() (T a) const {
        return std::sin(a); 
    }
};
