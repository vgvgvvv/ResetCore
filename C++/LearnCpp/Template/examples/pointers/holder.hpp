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
class Holder {
  private:
    T* ptr;    // refers to the object it holds (if any)

  public:
    // default constructor: let the holder refer to nothing
    Holder() : ptr(0) {
    }

    // constructor for a pointer: let the holder refer to where the pointer refers
    explicit Holder (T* p) : ptr(p) {
    }

    // destructor: releases the object to which it refers (if any)
    ~Holder() { 
        delete ptr;
    }

    // assignment of new pointer
    Holder<T>& operator= (T* p) {
        delete ptr;
        ptr = p;
        return *this;
    }

    // pointer operators
    T& operator* () const { 
        return *ptr;
    }

    T* operator-> () const { 
        return ptr; 
    }

    // get referenced object (if any)
    T* get() const {
        return ptr;
    }

    // release ownership of referenced object
    void release() {
        ptr = 0;
    }

    // exchange ownership with other holder
    void exchange_with (Holder<T>& h) {
        std::swap(ptr,h.ptr);
    }

    // exchange ownership with other pointer
    void exchange_with (T*& p) {
        std::swap(ptr,p);
    }

  private:
    // no copying and copy assignment allowed
    Holder (Holder<T> const&);
    Holder<T>& operator= (Holder<T> const&);
};
