#pragma once
#include "GlobalVars.h"

template <class T>
int GetId()
{
    static int s_componentId = s_componentCounter++;
    return s_componentId;
}