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
#ifndef SQRT_HPP
#define SQRT_HPP

#include "ifthenelse.hpp"

// template to yield template argument as result
template<int N>
class Value {
  public:
    enum { result = N };
};

// template to compute sqrt(N) via iteration
template <int N, int I=1>
class Sqrt {
  public:
    // instantiate next step or result type as branch
    typedef typename IfThenElse<(I*I<N),
                                Sqrt<N,I+1>,
                                Value<I>
                               >::ResultT
            SubT;

    // use the result of branch type
    enum { result = SubT::result };
};

#endif // SQRT_HPP
