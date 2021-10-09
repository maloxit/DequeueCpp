// Dequeue.cpp : Defines the entry point for the application.
//

#include "Dequeue.h"

using namespace std;

class SampleMemAllocStrategy
{
public:
	unsigned char* alloc(size_t size)
	{
		return (unsigned char*)malloc(size);
	}
	void delloc(unsigned char* data)
	{
		free(data);
	}
};


template<class DataType, class MemoryAllocationStrategy = SampleMemAllocStrategy>
class Deque
{

private:

	using DequeItem = struct _DequeItem
	{
		struct _DequeItem* next;
		struct _DequeItem* prev;
		DataType data;
	};

	DequeItem* front;
	DequeItem* back;

	MemoryAllocationStrategy memAllocator;

public:
	class DequeIterator
	{
	private:
		const Deque* ownerDeque;
		DequeItem* item;
	public:

		DequeIterator() = delete;

		DequeIterator(const Deque* ownerDeque, DequeItem* item) : ownerDeque(ownerDeque), item(item) { };

		DequeIterator& operator++()
		{
			if (item)
				item = item->next;
		}

		DequeIterator operator++(int)
		{
			DequeIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator!=(const DequeIterator& iterator)
		{
			return item != iterator.item || ownerDeque != iterator.ownerDeque;
		}

		DataType& operator*()
		{
			return item->data;
		}
	};

	class ConstDequeIterator
	{
	private:
		const DequeItem* item;
		const Deque* ownerDeque;
	public:

		ConstDequeIterator() = delete;

		ConstDequeIterator(const Deque* ownerDeque, const DequeItem* item) : item(item) { };

		ConstDequeIterator& operator++()
		{
			if (item)
				item = item->next;
		}

		ConstDequeIterator operator++(int)
		{
			DequeIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator!=(const ConstDequeIterator& iterator)
		{
			return item != iterator.item;
		}

		const DataType& operator*()
		{
			return item->data;
		}
	};

	DequeIterator begin()
	{
		return DequeIterator(front);
	}

	DequeIterator end()
	{
		return DequeIterator(nullptr);
	}

	ConstDequeIterator cbegin() const
	{
		return ConstDequeIterator(front);
	}

	ConstDequeIterator cend() const
	{
		return ConstDequeIterator(nullptr);
	}

	Deque() : front(nullptr), back(nullptr) { };
	Deque(Deque const& otherDequeue)
	{
		for (auto& data : otherDequeue)
		{
			PushBack(data);
		}
	}
	Deque(Deque&& otherDequeue)
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

	Deque& operator=(Deque const& otherDequeue)
	{
		Clear();
		for (auto& data : otherDequeue)
		{
			PushBack(data);
		}
		otherDequeue;
		return *this;
	}
	Deque& operator=(Deque&& otherDequeue)
	{
		Clear();
		front = otherDequeue.front;
		back = otherDequeue.back;
		otherDequeue.front = nullptr;
		otherDequeue.back = nullptr;
		return *this;
	}

	void PushFront(const DataType& data)
	{
		DequeItem* item = memAllocator.alloc(sizeof(DequeItem));
		item->data = data;
		item->prev = nullptr;
		item->next = front;
		if (front != nullptr)
		{
			front->prev = item
		}
		else
		{
			back = item;
		}
		front = item;
	}

	void PushBack(const DataType& data)
	{
		DequeItem* item = memAllocator.alloc(sizeof(DequeItem));
		item->data = data;
		item->next = nullptr;
		item->prev = back;
		if (back != nullptr)
		{
			back->next = item
		}
		else
		{
			front = item;
		}
		back = item;
	}

	void PushFront(DataType && data)
	{
		DequeItem* item = memAllocator.alloc(sizeof(DequeItem));
		item->data = std::move(data);
		item->prev = nullptr;
		item->next = front;
		if (front != nullptr)
		{
			front->prev = item
		}
		else
		{
			back = item;
		}
		front = item;
	}

	void PushBack(DataType&& data)
	{
		DequeItem* item = memAllocator.alloc(sizeof(DequeItem));
		item->data = std::move(data);
		item->next = nullptr;
		item->prev = back;
		if (back != nullptr)
		{
			back->next = item
		}
		else
		{
			front = item;
		}
		back = item;
	}

	void PopFront()
	{
		DequeItem* item = front;
		if (item == nullptr)
		{
			throw exception("Cannot delete from enpty deque.");
		}
		front = front->next;
		if (front == nullptr)
		{
			back = nullptr;
		}
		memAllocator.delloc(item);
	}
	void PopBack()
	{
		DequeItem* item = back;
		if (item == nullptr)
		{
			throw exception("Cannot delete from enpty deque.");
		}
		back = back->prev;
		if (back == nullptr)
		{
			front = nullptr;
		}
		memAllocator.delloc(item);
	}

	GetFront();
	GetBack();

	PeekFront();
	PeekBack();

	void Delete(DequeIterator& itarator)
	{

	}

	void Clear();


	Deque& operator+=(Deque& left, const Deque& right);

	Deque& operator+=(Deque& left, Deque&& right);



};

int main()
{
	cout << "Hello CMake." << endl;
	return 0;
}
