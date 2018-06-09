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
// primary template for type of Nth field of (duo) T
template <int N, typename T>
class DuoT {
  public:
    typedef void ResultT;    // in general, the result type is void
};

// specialization for 1st field of a plain duo
template <typename A, typename B>
class DuoT<1, Duo<A,B> > {
  public:
    typedef A ResultT;
};

// specialization for 2nd field of a plain duo
template <typename A, typename B>
class DuoT<2, Duo<A,B> > {
  public:
    typedef B ResultT;
};

// specialization for Nth field of a recursive duo
template <int N, typename A, typename B, typename C>
class DuoT<N, Duo<A, Duo<B,C> > > {
  public:
    typedef typename DuoT<N-1, Duo<B,C> >::ResultT ResultT;
};

// specialization for 1st field of a recursive duo
template <typename A, typename B, typename C>
class DuoT<1, Duo<A, Duo<B,C> > > {
  public:
    typedef A ResultT;
};

// specialization for 2nd field of a recursive duo
template <typename A, typename B, typename C>
class DuoT<2, Duo<A, Duo<B,C> > > {
  public:
    typedef B ResultT;
};
