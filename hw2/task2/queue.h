#pragma once
#include"list.h"

template <typename T>
class Queue : public List<T>
{       // 队列模板类（继承List原有接口）
public: // size()、empty()以及其它开放接口均可直接沿用
    void enqueue(T const& e)
    {
        List<T>::insertAsLast(e);
    }
    T dequeue() { return List<T>::remove(List<T>::first()); }
    T& front() { return List<T>::first()->data; }
};
