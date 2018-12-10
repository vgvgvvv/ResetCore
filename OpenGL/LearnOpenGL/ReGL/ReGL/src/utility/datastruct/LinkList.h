#ifndef __LINKLIST_H__
#define __LINKLIST_H__

namespace ReGL
{
    template <typename T>
    class LinkList
    {
    public:
        template <typename T>
        class Node
        {
        public:
            T value;
        private:
            Node* next_;
        };

        //TODO 构造函数、初始化、赋值、拷贝、取值符号

        T Get();
        void Add(T value);
        void Remove(T value);
        void RemoveAt(int index);
        void Clear();
        int IndexOf(T value);

    private:
        Node<T>* head_;
        Node<T>* tail_;

    };
}


#endif