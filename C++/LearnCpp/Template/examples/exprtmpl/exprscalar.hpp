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
// class for objects that represent scalars
template <typename T>
class A_Scalar {
  private:
    T const& s;  // value of the scalar

  public:
    // constructor initializes value
    A_Scalar (T const& v)
     : s(v) {
    }

    // for index operations the scalar is the value of each element
    T operator[] (size_t) const {
        return s;
    }

    // scalars have zero as size
    size_t size() const {
        return 0;
    };
};
