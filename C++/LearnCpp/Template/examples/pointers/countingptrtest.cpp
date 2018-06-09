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
#include "stdobjpolicy.hpp"
#include "stdarraypolicy.hpp"
#include "simplerefcount.hpp"
#include "countingptr.hpp"
#include <iostream>
#include <vector>

void test1();
void test2();

int main()
{
    test1();
    test2();
}

void test1()
{
    std::cout << "\ntest1():\n";
    CountingPtr<int> p0;
    { 
       CountingPtr<int> p1(new int(42));
       std::cout << "*p1: " << *p1 << std::endl;

       *p1 = 17;
       std::cout << "*p1: " << *p1 << std::endl;

       CountingPtr<int> p2 = p1;
       std::cout << "*p2: " << *p2 << std::endl;

       *p1 = 33;
       std::cout << "*p2: " << *p2 << std::endl;

       p0 = p2;
       std::cout << "*p0: " << *p0 << std::endl;

       ++*p0;
       ++*p1;
       ++*p2;
       std::cout << "*p0: " << *p0 << std::endl;
       std::cout << "*p1: " << *p1 << std::endl;
       std::cout << "*p2: " << *p2 << std::endl;
    }
    std::cout << "after block: *p0: " << *p0 << std::endl;
}

void test2()
{
    std::cout << "\ntest2():\n";
    { CountingPtr<int> p0(new int(42));
       CountingPtr<int> p2 = p0;
    }
    CountingPtr<int> p1(new int(42));
    
    std::cout << "qqq" << std::endl;

    std::vector<CountingPtr<int> > coll;
    std::cout << "qqq" << std::endl;
    coll.push_back(p1);
    std::cout << "qqq" << std::endl;
    coll.push_back(p1);
    std::cout << "qqq" << std::endl;

    std::cout << "qqq" << std::endl;

    ++*p1;
    ++*coll[0];
    std::cout << *coll[1] << std::endl;
}

