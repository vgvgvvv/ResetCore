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
template<typename T,
         typename CounterPolicy = SimpleReferenceCount,
         typename ObjectPolicy = StandardObjectPolicy>
class CountingPtr : private CounterPolicy, private ObjectPolicy {
  private:
    // shortcuts:
    typedef CounterPolicy CP;
    typedef ObjectPolicy  OP;

    T* object_pointed_to;      // the object referred to (or NULL if none)

  public:
    // default constructor (no explicit initialization):
    CountingPtr() {
        this->object_pointed_to = NULL;
    }

    // a converting constructor (from a built-in pointer):
    explicit CountingPtr (T* p) {
        this->init(p);         // init with ordinary pointer
    }

    // copy constructor:
    CountingPtr (CountingPtr<T,CP,OP> const& cp)
     : CP((CP const&)cp),      // copy policies
       OP((OP const&)cp) {
        this->attach(cp);      // copy pointer and increment counter
    }

    // destructor:
    ~CountingPtr() {
        this->detach();        // decrement counter
                               //  (and dispose counter if last owner)
    }

    // assignment of a built-in pointer
    CountingPtr<T,CP,OP>& operator= (T* p) {
        // no counting pointer should point to *p yet:
        assert(p != this->object_pointed_to);
        this->detach();        // decrement counter
                               //  (and dispose counter if last owner)
        this->init(p);         // init with ordinary pointer
        return *this;
    }

    // copy assignment (beware of self-assignment):
    CountingPtr<T,CP,OP>&
    operator= (CountingPtr<T,CP,OP> const& cp) {
        if (this->object_pointed_to != cp.object_pointed_to) {
            this->detach();    // decrement counter
                               //  (and dispose counter if last owner)
            CP::operator=((CP const&)cp);  // assign policies
            OP::operator=((OP const&)cp);
            this->attach(cp);  // copy pointer and increment counter
        }
        return *this;
    }

    // the operators that make this a smart pointer:
    T* operator-> () const {
        return this->object_pointed_to;
    }

    T& operator* () const {
        return *this->object_pointed_to;
    }

    // additional interfaces will be added later
    //...

  private:
    // helpers:
    // - init with ordinary pointer (if any)
    void init (T* p) {
        if (p != NULL) {
            CounterPolicy::init(p);
        }
        this->object_pointed_to = p;
    }

    // - copy pointer and increment counter (if any)
    void attach (CountingPtr<T,CP,OP> const& cp) {
        this->object_pointed_to = cp.object_pointed_to;
        if (cp.object_pointed_to != NULL) {
            CounterPolicy::increment(cp.object_pointed_to);
        }
    }

    // - decrement counter (and dispose counter if last owner)
    void detach() {
        if (this->object_pointed_to != NULL) {
            CounterPolicy::decrement(this->object_pointed_to);
            if (CounterPolicy::is_zero(this->object_pointed_to)) {
                // dispose counter, if necessary:
                CounterPolicy::dispose(this->object_pointed_to);
                // use object policy to dispose the object pointed to:
                ObjectPolicy::dispose(this->object_pointed_to);
            }
        }
    }
};
