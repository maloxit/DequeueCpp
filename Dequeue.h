#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include "defaultmemalloc.h"

template <class DataType, class MemoryAllocationStrategy = DefaultMemoryAllocationStrategy>
class Deque
{
  private:
    using DequeItem = struct _DequeItem
    {
        struct _DequeItem *next;
        struct _DequeItem *prev;
        DataType data;
    };

    DequeItem *front;
    DequeItem *back;

    MemoryAllocationStrategy memAllocator;

  public:
    class DequeIterator
    {
      private:
        Deque *ownerDeque;
        DequeItem *item;

      public:
        DequeIterator() = delete;

        DequeIterator(Deque *ownerDeque, DequeItem *item) : ownerDeque(ownerDeque), item(item){};

        DequeIterator &operator++()
        {
            if (item)
                item = item->next;
            return *this;
        }

        DequeIterator operator++(int)
        {
            DequeIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator!=(const DequeIterator &iterator)
        {
            if (ownerDeque != iterator.ownerDeque)
                throw std::logic_error("Unable to compare iterators of diferent deques.");
            return item != iterator.item;
        }

        DataType &operator*()
        {
            return item->data;
        }

        void Delete()
        {
            if (item == nullptr)
                throw std::logic_error("Unable to delete by end iterator.");

            if (item == ownerDeque->front && item == ownerDeque->back)
            {
                ownerDeque->front = nullptr;
                ownerDeque->back = nullptr;
            }
            else if (item == ownerDeque->front)
            {
                ownerDeque->front = ownerDeque->front->next;
                ownerDeque->front->prev = nullptr;
            }
            else if (item == ownerDeque->back)
            {
                ownerDeque->back = ownerDeque->back->prev;
                ownerDeque->back->next = nullptr;
            }
            else
            {
                item->prev->next = item->next;
                item->next->prev = item->prev;
            }
            auto tmp = item;
            ++(*this);
            ownerDeque->memAllocator.delloc((unsigned char *)tmp);
        }
    };

    class ConstDequeIterator
    {
      private:
        const DequeItem *item;
        const Deque *ownerDeque;

      public:
        ConstDequeIterator() = delete;

        ConstDequeIterator(const Deque *ownerDeque, const DequeItem *item) : item(item){};

        ConstDequeIterator &operator++()
        {
            if (item)
                item = item->next;
            return *this;
        }

        ConstDequeIterator operator++(int)
        {
            DequeIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator!=(const ConstDequeIterator &iterator)
        {
            if (ownerDeque != iterator.ownerDeque)
                throw std::logic_error("Unable to compare iterators of diferent deques.");
            return item != iterator.item;
        }

        const DataType &operator*()
        {
            return item->data;
        }
    };

    DequeIterator begin()
    {
        return DequeIterator(this, front);
    }

    DequeIterator end()
    {
        return DequeIterator(this, nullptr);
    }

    ConstDequeIterator begin() const
    {
        return ConstDequeIterator(this, front);
    }

    ConstDequeIterator end() const
    {
        return ConstDequeIterator(this, nullptr);
    }

    Deque() : front(nullptr), back(nullptr){};

    Deque(std::initializer_list<DataType> list) : Deque()
    {
        for (auto &data : list)
        {
            PushBack(data);
        }
    }

    Deque(Deque const &otherDequeue) : Deque()
    {
        for (auto &data : otherDequeue)
        {
            PushBack(data);
        }
    }

    Deque(Deque &&otherDequeue)
    {
        front = otherDequeue.front;
        back = otherDequeue.back;
        otherDequeue.front = nullptr;
        otherDequeue.back = nullptr;
    }

    ~Deque()
    {
        Clear();
    }

    Deque &operator=(Deque const &otherDequeue)
    {
        Clear();
        for (auto &data : otherDequeue)
        {
            PushBack(data);
        }
        otherDequeue;
        return *this;
    }
    Deque &operator=(Deque &&otherDequeue)
    {
        Clear();
        front = otherDequeue.front;
        back = otherDequeue.back;
        otherDequeue.front = nullptr;
        otherDequeue.back = nullptr;
        return *this;
    }

    void PushFront(const DataType &data)
    {
        DequeItem *item = (DequeItem *)memAllocator.alloc(sizeof(DequeItem));
        item->data = data;
        item->prev = nullptr;
        item->next = front;
        if (front != nullptr)
        {
            front->prev = item;
        }
        else
        {
            back = item;
        }
        front = item;
    }

    void PushBack(const DataType &data)
    {
        DequeItem *item = (DequeItem *)memAllocator.alloc(sizeof(DequeItem));
        item->data = data;
        item->next = nullptr;
        item->prev = back;
        if (back != nullptr)
        {
            back->next = item;
        }
        else
        {
            front = item;
        }
        back = item;
    }

    void PushFront(DataType &&data)
    {
        DequeItem *item = (DequeItem *)memAllocator.alloc(sizeof(DequeItem));
        item->data = std::move(data);
        item->prev = nullptr;
        item->next = front;
        if (front != nullptr)
        {
            front->prev = item;
        }
        else
        {
            back = item;
        }
        front = item;
    }

    void PushBack(DataType &&data)
    {
        DequeItem *item = (DequeItem *)memAllocator.alloc(sizeof(DequeItem));
        item->data = std::move(data);
        item->next = nullptr;
        item->prev = back;
        if (back != nullptr)
        {
            back->next = item;
        }
        else
        {
            front = item;
        }
        back = item;
    }

    void PopFront()
    {
        DequeItem *item = front;
        if (item == nullptr)
        {
            throw std::logic_error("Unable to delete from enpty deque.");
        }
        front = front->next;
        if (front == nullptr)
        {
            back = nullptr;
        }
        else
        {
            front->prev = nullptr;
        }
        memAllocator.delloc((unsigned char *)item);
    }

    void PopBack()
    {
        DequeItem *item = back;
        if (item == nullptr)
        {
            throw std::logic_error("Unable to delete from enpty deque.");
        }
        back = back->prev;
        if (back == nullptr)
        {
            front = nullptr;
        }
        else
        {
            back->next = nullptr;
        }
        memAllocator.delloc((unsigned char *)item);
    }

    DataType &GetFront()
    {
        if (front == nullptr)
            throw std::logic_error("Unable to get item from empty deque.");
        return front->data;
    }

    DataType &GetBack()
    {
        if (back == nullptr)
            throw std::logic_error("Unable to get item from empty deque.");
        return back->data;
    }

    DataType const &PeekFront() const
    {
        if (front == nullptr)
            throw std::logic_error("Unable to get item from empty deque.");
        return front->data;
    }

    DataType const &PeekBack() const
    {
        if (back == nullptr)
            throw std::logic_error("Unable to get item from empty deque.");
        return back->data;
    }

    void Clear()
    {
        DequeItem *item = front;
        while (item != nullptr)
        {
            auto tmp = item->next;
            memAllocator.delloc((unsigned char *)item);
            item = tmp;
        }
        front = nullptr;
        back = nullptr;
    }

    bool IsEmpty()
    {
        return front == nullptr;
    }

    Deque &operator+=(const Deque &right)
    {
        for (const DataType &data : right)
        {
            PushBack(data);
        }
        return *this;
    }

    Deque &operator+=(Deque &&right)
    {
        if (IsEmpty())
        {
            back = right.back;
            front = right.front;
        }
        else if (!right.IsEmpty())
        {
            back->next = right.front;
            right.front->prev = back;
            back = right.back;
        }
        right.front = nullptr;
        right.back = nullptr;
        return *this;
    }
};
