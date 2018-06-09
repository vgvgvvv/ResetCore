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
template<typename ObjectT,        // the class type containing the counter
         typename CountT,         // the type of the counter
         CountT ObjectT::*CountP> // the location of the counter
class MemberReferenceCount
{
  public:
    // the default constructor and destructor are fine

    // initialize the counter to one:
    void init (ObjectT* object) {
        object->*CountP = 1;
    }

    // no action is needed to dispose of the counter:
    void dispose (ObjectT*) {
    }

    // increment by one:
    void increment (ObjectT* object) {
        ++object->*CountP;
    }
    
    // decrement by one:
    void decrement (ObjectT* object) {
        --object->*CountP;
    }

    // test for zero:
    template<typename T> bool is_zero (ObjectT* object) {
        return object->*CountP == 0;
    }
};
