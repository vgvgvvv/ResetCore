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
#include "typeop.hpp"

template <typename T>
class BoundVal {
  private:
    T value;
  public:
    typedef T ValueT;
    BoundVal(T v) : value(v) {
    }
    typename TypeOp<T>::RefT get() { 
        return value; 
    }
};

template <typename T, T Val>
class StaticBoundVal {
  public:
    typedef T ValueT;
    T get() { 
        return Val; 
    }
};
