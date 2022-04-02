#pragma once
#include <iostream>
#include "PriorityInfo.h"
using namespace std;


template <class T1, class T2 = int>
struct node
{
    T2 priority;
    T1 info;
    node* link;
};

template <class T1, class T2 = int>
class priorityQueue
{
private:
    node<T1, T2>* front;
public:
    priorityQueue()
    {
        front = NULL;
    }

    void Push(T1 item, T2 priority)
    {
        node<T1, T2>* tmp, * q;
        tmp = new node<T1, T2>;
        tmp->info = item;
        tmp->priority = priority;
        if (front == NULL || priority < front->priority)
        {
            
            tmp->link = front;
            front = tmp;
        }
        else
        {
            q = front;
            while (q->link != NULL && q->link->priority <= priority)
                q = q->link;
            tmp->link = q->link;
            q->link = tmp;
        }
    }

    void CloneList(priorityQueue<T1, T2>* Copy) {
        
        node<T1, T2>* p = front;
        
        while (p) {
            Copy->Push(p->info, p->priority);
            p = p->link;
        }

    }

    void del()
    {
        node<T1, T2>* tmp;
        if (front == NULL)
            cout << "Queue Underflow\n";
        else
        {
            tmp = front;
            front = front->link;
            delete (tmp);
        }
    }

    void display()
    {
        node<T1, T2>* ptr;
        ptr = front;
        if (front == NULL)
            cout << "Queue is empty\n";
        else
        {
            cout << "Queue is :\n";
            cout << "Priority       Item\n";
            while (ptr != NULL)
            {
                cout << ptr->priority << "        "; ptr->info->print(); cout << endl;
                ptr = ptr->link;
            }
        }
    }
    
    bool empty() 
    {
        return front == nullptr;
    }

    bool getFront(T1& t1)
    {
        if (!this->empty())
        {
            t1 = front->info;
            del();
            return true;
        }
        return false;
    }

    bool peek(T1& t1)
    {
        if (!this->empty())
        {
            t1 = front->info;
            return true;
        }
        return false;
    }
    void operator=(priorityQueue& P1)
    {
        node<T1, T2>* p = P1->front;
        while (!empty())
        {
            del();
        }
        while (p)
        {
            Push(p->info, p->priority);
            p = p->link;
        }
    }


};