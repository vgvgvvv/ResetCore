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
#include "holder.hpp"

class RefMembers {
  private:
    Holder<MemType> ptr1;       // referenced members
    Holder<MemType> ptr2;
  public:
    // default constructor
    // - no resource leak possible
    RefMembers ()
     : ptr1(new MemType), ptr2(new MemType) {
    }

    // copy constructor
    // - no resource leak possible
    RefMembers (RefMembers const& x)
     : ptr1(new MemType(*x.ptr1)), ptr2(new MemType(*x.ptr2)) {
    }

    // assignment operator
    const RefMembers& operator= (RefMembers const& x) {
       *ptr1 = *x.ptr1;
       *ptr2 = *x.ptr2;
       return *this;
    }

    // no destructor necessary
    // (default destructor lets ptr1 and ptr2 delete their objects)
    //...
};

