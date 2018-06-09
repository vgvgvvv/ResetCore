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
template <typename FO1, typename FO2>
class Composer {
  private:
    FO1 fo1;  // first/inner function object to call
    FO2 fo2;  // second/outer function object to call
  public:
    // constructor: initialize function objects
    Composer (FO1 f1, FO2 f2)
     : fo1(f1), fo2(f2) {
    }
 
    // ``function call'': nested call of function objects
    double operator() (double v) {
        return fo2(fo1(v));
    }
};
