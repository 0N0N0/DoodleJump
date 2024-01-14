#pragma once
#include "../ECS/Utils/Entity.h"
#include "../ECS/Utils/ComponentNumerator.h"
#include "../ECS/Utils/ComponentPool.h"
#include "../ECS/Utils/GlobalVars.h"
#include <vector>

struct Scene
{
public:
    EntityID NewEntity();
    void DestroyEntity(EntityID id);

    std::vector<Entity> entities;
    std::vector<ComponentPool*> componentPools;
    std::vector<EntityIndex> freeEntities;

    template<typename T>
    T* Assign(EntityID id)
    {
        int componentId = GetId<T>();

        if (componentPools.size() <= componentId)
        {
            componentPools.resize(componentId + 1, nullptr);
        }
        if (componentPools[componentId] == nullptr)
        {
            componentPools[componentId] = new ComponentPool(sizeof(T));
        }

        T* pComponent = new (componentPools[componentId]->Get(GetEntityIndex(id))) T();

        entities[GetEntityIndex(id)].mask.set(componentId);
        return pComponent;
    }

    template<typename T>
    T* Get(EntityID id)
    {
        int componentId = GetId<T>();
        if (!entities[GetEntityIndex(id)].mask.test(componentId))
        {
            return nullptr;
        }

        T* pComponent = static_cast<T*>(componentPools[componentId]->Get(GetEntityIndex(id)));
        return pComponent;
    }

    template<typename T>
    void Remove(EntityID id)
    {
        if (entities[GetEntityIndex(id)].id != id)
        {
            return;
        }

        int componentId = GetId<T>();
        entities[GetEntityIndex(id)].mask.reset(componentId);
    }
};