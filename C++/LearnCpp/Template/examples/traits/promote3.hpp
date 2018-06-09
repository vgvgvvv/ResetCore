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
#define MK_PROMOTION(T1,T2,Tr)             \
    template<> class Promotion<T1, T2> {   \
      public:                              \
        typedef Tr ResultT;                \
    };                                     \
                                           \
    template<> class Promotion<T2, T1> {   \
      public:                              \
        typedef Tr ResultT;                \
    };
