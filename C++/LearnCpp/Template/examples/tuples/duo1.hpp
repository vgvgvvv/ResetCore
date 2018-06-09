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
#ifndef DUO_HPP
#define DUO_HPP

template <typename T1, typename T2>
class Duo {
  public:
    typedef T1 Type1;  // type of first field
    typedef T2 Type2;  // type of second field
    enum { N = 2 };    // number of fields
  
  private:
    T1 value1;         // value of first field
    T2 value2;         // value of second field
  
  public:
    // constructors
    Duo() : value1(), value2() {
    }
    Duo (T1 const & a, T2 const & b)
     : value1(a), value2(b) {
    }

    // for implicit type conversion during construction
    template <typename U1, typename U2>
    Duo (Duo<U1,U2> const & d)
     : value1(d.v1()), value2(d.v2()) {
    }

    // for implicit type conversion during assignments
    template <typename U1, typename U2>
    Duo<T1, T2>& operator = (Duo<U1,U2> const & d) {
        value1 = d.value1;
        value2 = d.value2;
        return *this;
    }

    // field access
    T1& v1() { 
        return value1;
    }
    T1 const& v1() const {
        return value1;
    }
    

    T2& v2() { 
        return value2;
    }
    T2 const& v2() const {
        return value2;
    }
};

// comparison operators (allow mixed types):
template <typename T1, typename T2,
          typename U1, typename U2>
inline
bool operator == (Duo<T1,T2> const& d1, Duo<U1,U2> const& d2)
{
    return d1.v1()==d2.v1() && d1.v2()==d2.v2();
}

template <typename T1, typename T2,
          typename U1, typename U2>
inline
bool operator != (Duo<T1,T2> const& d1, Duo<U1,U2> const& d2)
{
    return !(d1==d2);
}

// convenience function for creation and initialization
template <typename T1, typename T2> 
inline
Duo<T1,T2> make_duo (T1 const & a, T2 const & b)
{
    return Duo<T1,T2>(a,b);
}

#endif // DUO_HPP
