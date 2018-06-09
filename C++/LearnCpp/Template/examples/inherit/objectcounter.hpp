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
#include <stddef.h>

template <typename CountedType>
class ObjectCounter {
  private:
    static size_t count;    // number of existing objects

  protected:
    // default constructor
    ObjectCounter() { 
        ++count;
    }

    // copy constructor
    ObjectCounter (ObjectCounter<CountedType> const&) {
        ++count; 
    }

    // destructor
    ~ObjectCounter() { 
        --count;
    }

  public:
    // return number of existing objects:
    static size_t live() { 
        return count; 
    }
};

// initialize counter with zero
template <typename CountedType>
size_t ObjectCounter<CountedType>::count = 0;
