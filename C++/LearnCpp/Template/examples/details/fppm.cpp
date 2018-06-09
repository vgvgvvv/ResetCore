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
template <int N>
class X {
  public:
    typedef int I;
    void f(int) {
    }
};

template<int N> 
void fppm(void (X<N>::*p)(typename X<N>::I));

int main() 
{
    fppm(&X<33>::f);  // fine: N deduced to be 33
}
