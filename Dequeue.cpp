#include "Dequeue.h"

void PrintDeque(const Deque<int> &deque)
{
    for (auto &i : deque)
    {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
}

void PrintInt(int i)
{
    std::cout << i << "  ";
    std::cout << std::endl;
}

int main()
{
    //Test: default constructor
    Deque<int> deque0;
    PrintDeque(deque0);

    //Test: initializer_list constructor
    Deque<int> deque1({1, 1, 2, 3, 5});
    PrintDeque(deque1);

    //Test: move constructor
    Deque<int> deque2(Deque<int>({1, 1, 2, 3, 5}));
    PrintDeque(deque2);

    //Test: copying constructor
    Deque<int> deque3(deque1);
    PrintDeque(deque3);

    //Test: copying operator
    Deque<int> deque4 = deque1;
    PrintDeque(deque4);

    //Test: move operator
    Deque<int> deque5 = Deque<int>({1, 1, 2, 3, 5});
    PrintDeque(deque5);

    //Test: concat copying operator
    deque4 += deque5;
    PrintDeque(deque4);

    //Test: concat move operator
    deque4 += Deque<int>({42, 561});
    PrintDeque(deque4);

    //Test: get front
    PrintInt(deque4.GetFront());
    PrintDeque(deque4);

    //Test: get back
    PrintInt(deque4.GetBack());
    PrintDeque(deque4);

    //Test: delete front
    deque4.PopFront();
    PrintDeque(deque4);

    //Test: delete back
    deque4.PopBack();
    PrintDeque(deque4);

    //Test: delete in middle by iterator
    auto iterator = deque4.begin();
    auto iterEnd = deque4.end();

    while (iterator != iterEnd)
    {
        if (*iterator == 2)
            iterator.Delete();
        else
            ++iterator;
    }
    PrintDeque(deque4);

    return 0;
}
