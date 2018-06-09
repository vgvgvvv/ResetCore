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
/* helper traits class to select how to refer to an ``expression template node''
 * - in general: by reference
 * - for scalars: by value
 */

template <typename T> class A_Scalar;

// primary template
template <typename T>
class A_Traits {
  public:
    typedef T const& ExprRef;     // type to refer to is constant reference
};

// partial specialization for scalars
template <typename T>
class A_Traits<A_Scalar<T> > {
  public:
    typedef A_Scalar<T> ExprRef;  // type to refer to is ordinary value
};
