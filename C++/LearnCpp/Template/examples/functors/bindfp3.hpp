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
// convenience function to bind a function pointer with three parameters
template<int PNum, typename RT, typename P1, typename P2, typename P3>
inline
Binder<FunctionPtr<RT,P1,P2,P3>,
       PNum,
       BoundVal<typename FunctorParam<FunctionPtr<RT,P1,P2,P3>,
                                      PNum
                                     >::Type
               >
      >
bindfp (RT (*fp)(P1,P2,P3),
        typename ForwardParamT<typename FunctorParam<FunctionPtr<RT,P1,P2,P3>,
                                                     PNum
                                                    >::Type  
                              >::Type val)
{
    return Binder<FunctionPtr<RT,P1,P2,P3>, PNum,
                  BoundVal<typename FunctorParam<FunctionPtr<RT,P1,P2,P3>,
                                                             PNum
                                                >::Type>
                          >(func_ptr(fp),
                            BoundVal<typename FunctorParam
                                       <FunctionPtr<RT,P1,P2,P3>,
                                        PNum
                                       >::Type
                                    >(val));
}
