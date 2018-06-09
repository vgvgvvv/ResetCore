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
// partial specialization for Duo<> with only one field
template <typename A>
struct Duo<A,void> {
  public:
    typedef A    T1;  // type of first field
    typedef void T2;  // type of second field
    enum { N = 1 };   // number of fields

  private:
    T1 value1;        // value of first field

  public:
    // constructors
    Duo() : value1() {
    }
    Duo (T1 const & a)
     : value1(a) {
    }

    // field access
    T1& v1() { 
        return value1;
    }
    T1 const& v1() const {
        return value1;
    }
    
    void v2() { 
    }
    void v2() const {
    }
    //...
};
