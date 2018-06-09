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
// prime number computation by Erwin Unruh

template <int p, int i>
class is_prime {
  public:
    enum { prim = (p==2) || (p%i) && is_prime<(i>2?p:0),i-1>::prim
         }; 
}; 

template<>
class is_prime<0,0> {
  public:
    enum {prim=1};
}; 

template<>
class is_prime<0,1> {
  public:
    enum {prim=1};
}; 

template <int i>
class D {
  public:
    D(void*);
}; 

template <int i>
class Prime_print {      // primary template for loop to print prime numbers
  public:
    Prime_print<i-1> a; 
    enum { prim = is_prime<i,i-1>::prim
         }; 
    void f() {
        D<i> d = prim ? 1 : 0;
        a.f();
    } 
}; 

template<>
class Prime_print<1> {   // full specialization to end the loop
  public:
    enum {prim=0}; 
    void f() {
        D<1> d = prim ? 1 : 0;
    }; 
}; 

#ifndef LAST 
#define LAST 18 
#endif 

int main()
{
    Prime_print<LAST> a; 
    a.f(); 
} 
