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
#include "boundval.hpp"
#include "forwardparam.hpp"
#include "functorparam.hpp"
#include "binderparams.hpp"
#include "signselect.hpp"

template <typename FO, int P, typename V>
class Binder : private FO, private V {
  public:
    // there is one less parameter because one is bound:
    enum { NumParams = FO::NumParams-1 };
    // the return type is straightforward:
    typedef typename FO::ReturnT ReturnT;

    // the parameter types:
    typedef BinderParams<FO, P> Params;
#define ComposeParamT(N)                                          \
        typedef typename                                          \
                ForwardParamT<typename Params::Param##N##T>::Type \
            Param##N##T
    ComposeParamT(1);
    ComposeParamT(2);
    ComposeParamT(3);
    //...
#undef ComposeParamT

    // constructors:
    Binder(FO& f): FO(f) {}
    Binder(FO& f, V& v): FO(f), V(v) {}
    Binder(FO& f, V const& v): FO(f), V(v) {}
    Binder(FO const& f): FO(f) {}
    Binder(FO const& f, V& v): FO(f), V(v) {}
    Binder(FO const& f, V const& v): FO(f), V(v) {}
    template<class T>
      Binder(FO& f, T& v): FO(f), V(BoundVal<T>(v)) {}
    template<class T>
      Binder(FO& f, T const& v): FO(f), V(BoundVal<T const>(v)) {}

    // ``function calls'':
    ReturnT operator() () {
        return FO::operator()(V::get());
    }
    ReturnT operator() (Param1T v1) {
        return FO::operator()(ArgSelect<1>::from(v1,v1,V::get()),
                              ArgSelect<2>::from(v1,v1,V::get()));
    }
    ReturnT operator() (Param1T v1, Param2T v2) {
        return FO::operator()(ArgSelect<1>::from(v1,v1,V::get()),
                              ArgSelect<2>::from(v1,v2,V::get()),
                              ArgSelect<3>::from(v2,v2,V::get()));
    }
    ReturnT operator() (Param1T v1, Param2T v2, Param3T v3) {
        return FO::operator()(ArgSelect<1>::from(v1,v1,V::get()),
                              ArgSelect<2>::from(v1,v2,V::get()),
                              ArgSelect<3>::from(v2,v3,V::get()),
                              ArgSelect<4>::from(v3,v3,V::get()));
    }
    //...

  private:
    template<int A>
    class ArgSelect {
      public:
        // type if we haven't passed the bound argument yet:
        typedef typename TypeOp<
                    typename IfThenElse<(A<=Params::NumParams),
                                        FunctorParam<Params, A>,
                                        FunctorParam<Params, A-1>
                                       >::ResultT::Type>::RefT
                NoSkipT;
        // type if we're past the bound argument:
        typedef typename TypeOp<
                    typename IfThenElse<(A>1),
                                        FunctorParam<Params, A-1>,
                                        FunctorParam<Params, A>
                                       >::ResultT::Type>::RefT
                SkipT;
        // type of bound argument:
        typedef typename TypeOp<typename V::ValueT>::RefT BindT;

        // three selection cases implemented through different classes:
        class NoSkip {
          public:
            static NoSkipT select (SkipT prev_arg, NoSkipT arg,
                                   BindT bound_val) {
                return arg;
            }
        };
        class Skip {
          public:
            static SkipT select (SkipT prev_arg, NoSkipT arg,
                                 BindT bound_val) {
                return prev_arg;
            }
        };
        class Bind {
          public:
            static BindT select (SkipT prev_arg, NoSkipT arg,
                                 BindT bound_val) {
                return bound_val;
            }
        };

        // the actual selection function:
        typedef typename SignSelectT<A-P, NoSkipT,
                                     BindT, SkipT>::ResultT
                ReturnT;
        typedef typename SignSelectT<A-P, NoSkip,
                                     Bind, Skip>::ResultT
                SelectedT;
        static ReturnT from (SkipT prev_arg, NoSkipT arg, 
                             BindT bound_val) {
            return SelectedT::select (prev_arg, arg, bound_val);
        }
    };
};
