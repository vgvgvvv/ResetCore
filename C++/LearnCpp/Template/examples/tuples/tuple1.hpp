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
#include "duo1.hpp"
#include "duo2.hpp"
#include "duo3.hpp"
#include "duo4.hpp"
#include "duo5.hpp"
#include "duo6.hpp"
#include "typeop.hpp"

// type that represents unused type parameters
class NullT {
};

// Tuple<> in general derives from Tuple<> with one more NullT
template <typename P1,
          typename P2 = NullT,
          typename P3 = NullT,
          typename P4 = NullT,
          typename P5 = NullT>
class Tuple
 : public Duo<P1, typename Tuple<P2,P3,P4,P5,NullT>::BaseT> {
  public:
    typedef Duo<P1, typename Tuple<P2,P3,P4,P5,NullT>::BaseT>
            BaseT;

    // constructors:
    Tuple() {}
    Tuple(typename TypeOp<P1>::RefConstT a1,
          typename TypeOp<P2>::RefConstT a2,
          typename TypeOp<P3>::RefConstT a3 = NullT(),
          typename TypeOp<P4>::RefConstT a4 = NullT(),
          typename TypeOp<P5>::RefConstT a5 = NullT())
     : BaseT(a1, Tuple<P2,P3,P4,P5,NullT>(a2,a3,a4,a5)) {
    }
};

// specialization to end deriving recursion
template <typename P1, typename P2>
class Tuple<P1,P2,NullT,NullT,NullT> : public Duo<P1,P2> {
  public:
    typedef Duo<P1,P2> BaseT;
    Tuple() {}
    Tuple(typename TypeOp<P1>::RefConstT a1,
          typename TypeOp<P2>::RefConstT a2,
          typename TypeOp<NullT>::RefConstT = NullT(),
          typename TypeOp<NullT>::RefConstT = NullT(),
          typename TypeOp<NullT>::RefConstT = NullT())
     : BaseT(a1, a2) {
    }
};

// specialization for singletons
template <typename P1>
class Tuple<P1,NullT,NullT,NullT,NullT> : public Duo<P1,void> {
  public:
    typedef Duo<P1,void> BaseT;
    Tuple() {}
    Tuple(typename TypeOp<P1>::RefConstT a1,
          typename TypeOp<NullT>::RefConstT = NullT(),
          typename TypeOp<NullT>::RefConstT = NullT(),
          typename TypeOp<NullT>::RefConstT = NullT(),
          typename TypeOp<NullT>::RefConstT = NullT())
     : BaseT(a1) {
    }
};

// convenience function for 1 argument
template <typename T1>
inline
Tuple<T1> make_tuple(T1 const &a1)
{
    return Tuple<T1>(a1);
}

// convenience function for 2 arguments
template <typename T1, typename T2>
inline
Tuple<T1,T2> make_tuple(T1 const &a1, T2 const &a2)
{
    return Tuple<T1,T2>(a1,a2);
}

// convenience function for 3 arguments
template <typename T1, typename T2, typename T3>
inline
Tuple<T1,T2,T3> make_tuple(T1 const &a1, T2 const &a2,
                           T3 const &a3)
{
    return Tuple<T1,T2,T3>(a1,a2,a3);
}

// convenience function for 4 arguments
template <typename T1, typename T2, typename T3, typename T4>
inline
Tuple<T1,T2,T3,T4> make_tuple(T1 const &a1, T2 const &a2, 
                              T3 const &a3, T4 const &a4)
{
    return Tuple<T1,T2,T3,T4>(a1,a2,a3,a4);
}

// convenience function for 5 arguments
template <typename T1, typename T2, typename T3,
          typename T4, typename T5>
inline
Tuple<T1,T2,T3,T4,T5> make_tuple(T1 const &a1, T2 const &a2,
                                 T3 const &a3, T4 const &a4,
                                 T5 const &a5)
{
    return Tuple<T1,T2,T3,T4,T5>(a1,a2,a3,a4,a5);
}
