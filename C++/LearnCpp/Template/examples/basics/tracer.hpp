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
#include <iostream>

class SortTracer {
  private:
    int value;                // integer value to be sorted
    int generation;           // generation of this tracer
    static long n_created;    // number of constructor calls
    static long n_destroyed;  // number of destructor calls
    static long n_assigned;   // number of assignments
    static long n_compared;   // number of comparisons
    static long n_max_live;   // maximum of existing objects

    // recompute maximum of existing objects
    static void update_max_live() {
        if (n_created-n_destroyed > n_max_live) {
            n_max_live = n_created-n_destroyed;
        }
    }

  public:
    static long creations() { 
        return n_created; 
    }
    static long destructions() { 
        return n_destroyed; 
    }
    static long assignments() { 
        return n_assigned; 
    }
    static long comparisons() { 
        return n_compared; 
    }
    static long max_live() { 
        return n_max_live; 
    }

  public:
    // constructor
    SortTracer (int v = 0) : value(v), generation(1) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracer #" << n_created
                  << ", created generation " << generation
                  << " (total: " << n_created - n_destroyed
                  << ")\n";
    }
   
    // copy constructor
    SortTracer (SortTracer const& b) 
     : value(b.value), generation(b.generation+1) {
        ++n_created;
        update_max_live();
        std::cerr << "SortTracer #" << n_created
                  << ", copied as generation " << generation
                  << " (total: " << n_created - n_destroyed
                  << ")\n";
    }

    // destructor
    ~SortTracer() {
        ++n_destroyed;
        update_max_live();
        std::cerr << "SortTracer generation " << generation
                  << " destroyed (total: " 
                  << n_created - n_destroyed << ")\n";
    }

    // assignment
    SortTracer& operator= (SortTracer const& b) {
        ++n_assigned;
        std::cerr << "SortTracer assignment #" << n_assigned 
                  << " (generation " << generation
                  << " = " << b.generation 
                  << ")\n";
        value = b.value;
        return *this;
    }

    // comparison
    friend bool operator < (SortTracer const& a, 
                            SortTracer const& b) {
        ++n_compared;
        std::cerr << "SortTracer comparison #" << n_compared 
                  << " (generation " << a.generation
                  << " < " << b.generation 
                  << ")\n";
        return a.value < b.value;
    }

    int val() const { 
        return value;
    }
};
