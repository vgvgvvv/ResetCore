//
// Created by 35207 on 2019/11/15 0015.
//

#ifndef RESETCORE_POLICYCLASS_H
#define RESETCORE_POLICYCLASS_H


#include <iostream>

template <typename T>
class OpNewCreator1{
public:
    static T* Create(){
        std::cout << "Type one" << std::endl;
        return new T;
    }
};

template <typename T>
class OpNewCreator2{
public:
    static T* Create(){
        std::cout << "Type two" << std::endl;
        return new T;
    }
};

class Test{

};

template <class T, template <class> class CreationPolicy>
class TestManager : public CreationPolicy<T>{

public:
    T* Create(){
        return CreationPolicy<T>::Create();
    }


};

#endif //RESETCORE_POLICYCLASS_H
