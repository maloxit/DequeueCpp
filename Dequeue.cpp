// Dequeue.cpp : Defines the entry point for the application.
//

#include "Dequeue.h"

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
			return *this;
		}

		DequeIterator operator++(int)
		{
			DequeIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator!=(const DequeIterator& iterator)
		{
			if (ownerDeque != iterator.ownerDeque)
			  throw std::logic_error("Unable to compare iterators of diferent deques.");
			return item != iterator.item;
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
			return *this;
		}

		ConstDequeIterator operator++(int)
		{
			DequeIterator tmp(*this);
			++(*this);
			return tmp;
		}

		bool operator!=(const ConstDequeIterator& iterator)
		{
			if (ownerDeque != iterator.ownerDeque)
			  throw std::logic_error("Unable to compare iterators of diferent deques.");
			return item != iterator.item;
		}

		const DataType& operator*()
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

	ConstDequeIterator cbegin() const
	{
		return ConstDequeIterator(this, front);
	}

	ConstDequeIterator cend() const
	{
		return ConstDequeIterator(this, nullptr);
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
		DequeItem* item = (DequeItem*)memAllocator.alloc(sizeof(DequeItem));
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

	void PushBack(const DataType& data)
	{
		DequeItem* item = (DequeItem*)memAllocator.alloc(sizeof(DequeItem));
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

	void PushFront(DataType && data)
	{
		DequeItem* item = (DequeItem*)memAllocator.alloc(sizeof(DequeItem));
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

	void PushBack(DataType&& data)
	{
		DequeItem* item = (DequeItem*)memAllocator.alloc(sizeof(DequeItem));
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
		DequeItem* item = front;
		if (item == nullptr)
		{
			throw std::logic_error("Unable to delete from enpty deque.");
		}
		front = front->next;
		if (front == nullptr)
		{
			back = nullptr;
		}
		memAllocator.delloc((unsigned char*)item);
	}
	void PopBack()
	{
		DequeItem* item = back;
		if (item == nullptr)
		{
			throw std::logic_error("Unable to delete from enpty deque.");
		}
		back = back->prev;
		if (back == nullptr)
		{
			front = nullptr;
		}
		memAllocator.delloc((unsigned char*)item);
	}

	DataType& GetFront()
	{
	  if (front == nullptr)
	    throw std::logic_error("Unable to get item from empty deque.");
	  return front->data;
	}
	
	DataType& GetBack()
	{
	  if (back == nullptr)
	    throw std::logic_error("Unable to get item from empty deque.");
	  return back->data;
	}

	DataType const & PeekFront() const
	{
	  if (front == nullptr)
	    throw std::logic_error("Unable to get item from empty deque.");
	  return front->data;  
	}
	
	DataType const & PeekBack() const
	{
	  if (back == nullptr)
	    throw std::logic_error("Unable to get item from empty deque.");
	  return back->data; 
	}

	void Delete(DequeIterator& iterator)
	{
	  DequeItem* item = iterator.item;
	  if (this != iterator.owner)
	    throw std::logic_error("Unable to delete by iterator of another deque.");
	  if (item == nullptr)
	    throw std::logic_error("Unable to delete by end iterator.");
	  if (item == front && item == back)
	  {
	    front = nullptr;
	    back = nullptr;
	  }
	  else if (item == front)
	  {
	    front = front->next;
	    front->prev = nullptr;
	  }
	  else if (item == back)
	  {
	    back = back->prev;
	    back->next = nullptr;
	  }
	  else
	  {
	    item->back->next = item->next;
	    item->next->back = item->back;
	  }
	  memAllocator.delloc((unsigned char*)item);
	}

	void Clear()
	{
	  DequeItem* item = front;
	  while (item != nullptr)
	  {
	    auto tmp = item->next;
	    memAllocator.delloc((unsigned char*)item);
	    item = tmp;
	  }
	  front = nullptr;
	  back = nullptr;
	}
	
	bool IsEmpty()
	{
	  return front == nullptr;
	}


	Deque& operator+=(const Deque& right)
	{
	  for (const DataType &data: right)
	  {
	    PushBack(data);
	  }
	  return *this;
	}

	Deque& operator+=(Deque&& right)
	{
	  if (IsEmpty())
	  {
	    back = right.back;
	    front = right.front;
	  }
	  else if (!right.IsEmpy())
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

int main()
{
	Deque<int> deque;
	for (int i = 0; i < 10; i++)
	{
		deque.PushBack(i);
	}
	std::cout << "Hello CMake." << std::endl;
	for (auto& i : deque)
	{
		std::cout << i << std::endl;
	}
	return 0;
}
