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
template<typename T>
class IsClassT {
  public:
    enum { Yes = IsFundaT<T>::No &&
                 IsEnumT<T>::No &&
                 !CompoundT<T>::IsPtrT &&
                 !CompoundT<T>::IsRefT &&
                 !CompoundT<T>::IsArrayT &&
                 !CompoundT<T>::IsPtrMemT &&
                 !CompoundT<T>::IsFuncT };
    enum { No = !Yes };
};
