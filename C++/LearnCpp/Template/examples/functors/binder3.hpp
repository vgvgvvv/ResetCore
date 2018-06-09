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
    //...
    ReturnT operator() (Param1T v1, Param2T v2, Param3T v3) {
        return FO::operator()(ArgSelect<1>::from(v1,v1,V::get()),
                              ArgSelect<2>::from(v1,v2,V::get()),
                              ArgSelect<3>::from(v2,v3,V::get()),
                              ArgSelect<4>::from(v3,v3,V::get()));
    }
    //...
};
