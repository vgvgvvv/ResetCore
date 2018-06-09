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
template <typename T> 
inline T const& max (T const& a, T const& b)
{
    return  a < b ? b : a;
}

int main()
{
    max<double>(1.0, -3.0);  // explicitly specify template argument
    max(1.0, -3.0);          // template argument is implicitly deduced
                             // to be double
    max<int>(1.0, 3.0);      // the explicit <int> inhibits the deduction;
                             // hence the result has type int
}

