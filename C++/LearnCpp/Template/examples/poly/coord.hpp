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
#include <cstdlib>

class Coord {
  private:
    int x, y;
  public:
    Coord (int i1, int i2) : x(i1), y(i2) {
    }
    friend Coord operator- (Coord const& c1, Coord const& c2) {
        return Coord(c1.x-c2.x, c1.y-c2.y);
    }
    Coord abs() {
        return Coord(std::abs(x),std::abs(y));
    }
};
