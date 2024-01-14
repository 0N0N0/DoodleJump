#pragma once
#include "GlobalVars.h"
#include <bitset>

typedef unsigned long long EntityID;
typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

struct Entity
{
    EntityID id;
    ComponentMask mask;
};


inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
    return ((EntityID)index << 32) | ((EntityID)version);
}
inline EntityIndex GetEntityIndex(EntityID id)
{
    return id >> 32;
}
inline EntityVersion GetEntityVersion(EntityID id)
{
    return (EntityVersion)id;
}
inline bool IsEntityValid(EntityID id)
{
    return (id >> 32) != EntityIndex(-1);
}

#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)