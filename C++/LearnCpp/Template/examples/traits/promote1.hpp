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
// primary template for type promotion
template<typename T1, typename T2>
class Promotion {
  public:
    typedef typename 
            IfThenElse<(sizeof(T1)>sizeof(T2)),
                       T1,
                       typename IfThenElse<(sizeof(T1)<sizeof(T2)),
                                           T2,
                                           void
                                          >::ResultT
                      >::ResultT ResultT;
};
