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
template <typename FO, int P, typename V>
class Binder : private FO, private V {
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
