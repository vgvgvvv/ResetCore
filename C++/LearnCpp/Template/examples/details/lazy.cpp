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
class Safe {
};

template <int N>
class Danger {
  public:
    typedef char Block[N];  // would fail for N<=0
};

template <typename T, int N>
class Tricky {
  public:
    virtual ~Tricky() {
    }
    void no_body_here(Safe<T> = 3);
    void inclass() { 
        Danger<N> no_boom_yet;
    }
    // \Tvoid error() \ Danger<0> boom; \
    // void unsafe(T (*p)[N]);
    T operator->();
    // virtual Safe<T> suspect();
    struct Nested { 
        Danger<N> pfew; 
    };
    union {  // anonymous union
       int align;
       Safe<T> anonymous;
    };
};

int main()
{
   Tricky<int, 0> ok;
}
