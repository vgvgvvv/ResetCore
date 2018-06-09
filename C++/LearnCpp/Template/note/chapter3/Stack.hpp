//
// Created by ¶­å· on 2018/6/10.
//

#ifndef LEARNCPP_STACK_HPP
#define LEARNCPP_STACK_HPP

#include <stdexcept>

template <typename T, int MAXSIZE>
class Stack {
private:
    T elems[MAXSIZE];
    int numElems;
public:
    Stack();
    void push(T const&);
    void pop();
    T top() const;
    bool empty() const{
        return numElems == 0;
    };
    bool full() const{
        return numElems == MAXSIZE;
    }
};

template <typename T, int MAXSIZE>
Stack<T, MAXSIZE>::Stack() : numElems(0) {

}

template <typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::push(const T& elem) {
    if(numElems == MAXSIZE){
        throw std::out_of_range("stack is full");
    }
    elems[numElems] = elem;
    ++numElems;
}

template <typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::pop() {
    if(numElems <= 0){
        throw std::out_of_range("stack is empty");
    }
    --numElems;
}

template <typename T, int MAXSIZE>
T Stack<T, MAXSIZE>::top() const {
    if(numElems <= 0){
        throw std::out_of_range("stack is empty");
    }
    return elems[numElems - 1];
}


#endif //LEARNCPP_STACK_HPP
