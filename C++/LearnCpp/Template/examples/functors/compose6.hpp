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
#include "forwardparam.hpp"
#include "functorparam.hpp"

template <typename C, int N> 
class BaseMem : public C {
  public:
    BaseMem(C& c) : C(c) { }
    BaseMem(C const& c) : C(c) { }
};

template <typename FO1, typename FO2>
class Composer : private BaseMem<FO1,1>,
                 private BaseMem<FO2,2> {
  public:
    // to let it fit in our framework:
    enum { NumParams = FO1::NumParams };
    typedef typename FO2::ReturnT ReturnT;

    // define Param1T, Param2T, and so on
    // - use a macro to ease the replication of the parameter type construct
#define ComposeParamT(N)                                            \
        typedef typename FunctorParam<FO1, N>::Type Param##N##T
    ComposeParamT(1);
    ComposeParamT(2);
    //...
#undef ComposeParamT

    // constructors: initialize function objects
    Composer(FO1 const& f1, FO2 const& f2)
     : BaseMem<FO1,1>(f1), BaseMem<FO2,2>(f2) {
    }
    Composer(FO1 const& f1, FO2& f2)
     : BaseMem<FO1,1>(f1), BaseMem<FO2,2>(f2) {
    }
    Composer(FO1& f1, FO2 const& f2)
     : BaseMem<FO1,1>(f1), BaseMem<FO2,2>(f2) {
    }
    Composer(FO1& f1, FO2& f2)
     : BaseMem<FO1,1>(f1), BaseMem<FO2,2>(f2) {
    }

    // ``function call'' for no argument:
    ReturnT operator() () {
      return BaseMem<FO2,2>::operator()(BaseMem<FO1,1>::operator()());
    }

    // ``function call'' for one argument:
    ReturnT operator() (typename ForwardParamT<Param1T>::Type v1) {
        return BaseMem<FO2,2>::operator()(BaseMem<FO1,1>::operator()(v1));
    }

    // ``function call'' for two arguments:
    ReturnT operator() (typename ForwardParamT<Param1T>::Type v1,
                        typename ForwardParamT<Param2T>::Type v2) {
        return BaseMem<FO2,2>::operator()(BaseMem<FO1,1>::operator()(v1, v2));
    }
    //...
};
