#pragma once
#include "GlobalVars.h"

struct ComponentPool
{
    ComponentPool(size_t elementsize)
    {
        elementSize = elementsize;
        pData = new char[elementSize * MAX_ENTITIES];
    }

    ~ComponentPool()
    {
        delete[] pData;
    }

    inline void* Get(size_t index)
    {
        return pData + index * elementSize;
    }

    char* pData{ nullptr };
    size_t elementSize{ 0 };
};