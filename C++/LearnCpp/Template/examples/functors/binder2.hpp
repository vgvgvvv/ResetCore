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
  //...
};
