#pragma once

#include <cstdlib>

class SampleMemAllocStrategy
{
  public:
    unsigned char *alloc(size_t size)
    {
        return (unsigned char *)malloc(size);
    }
    void delloc(unsigned char *data)
    {
        free(data);
    }
};

using DefaultMemoryAllocationStrategy = SampleMemAllocStrategy;