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
#include "ifthenelse.hpp"

template<typename F, int P>
class BinderParams {
  public:
    // there is one less parameter because one is bound:
    enum { NumParams = F::NumParams-1 };
#define ComposeParamT(N)                                       \
    typedef typename IfThenElse<(N<P), FunctorParam<F, N>,     \
                                       FunctorParam<F, N+1>    \
                               >::ResultT::Type                \
            Param##N##T
    ComposeParamT(1);
    ComposeParamT(2);
    ComposeParamT(3);
    //...
#undef ComposeParamT
};

