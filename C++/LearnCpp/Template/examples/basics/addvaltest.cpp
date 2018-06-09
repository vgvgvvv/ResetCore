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
#include "addval.hpp"

void call_addValue()
{
    addValue<int,5>(42);
}

template <typename IT, typename OP>
void transform (IT beg, IT end, IT to, OP op)
{
    while (beg != end) {
        *to++ = op(*beg++);
    }
}

int main()
{
    call_addValue();

    int m[] = { 1, 2, 3, 4, 5, 6 };

    transform (m, m+6, 
               m,
#ifndef CORE_ISSUE_115_SOLVED
               (int(*)(int const&))    // cast necessary
#endif
               addValue<int,5>);
}

