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
#include "coord.hpp"

// concrete geometric object class Circle
// - \bfseries not derived from any class
class Circle {
  public:
    void draw() const;
    Coord center_of_gravity() const;
    //...
};

// concrete geometric object class Line
// - \bfseries not derived from any class
class Line {
  public:
    void draw() const;
    Coord center_of_gravity() const;
    //...
};
//...
