#pragma once
#include "../Components/PositionComponent.h"
#include "../Components/CollisionComponent.h"
#include <iostream>

struct CheckCollision
{
    static std::string CheckCollisionFunc(PositionComponent* pPositionComponent, CollisionComponent* pCollisionComponent, PositionComponent* pPositionComponent2, CollisionComponent* pCollisionComponent2)
    {
        float left1 = pPositionComponent->position.x;
        float top1 = pPositionComponent->position.y;
        float right1 = pPositionComponent->position.x + pCollisionComponent->size.x;
        float bottom1 = pPositionComponent->position.y + pCollisionComponent->size.y;

        float left2 = pPositionComponent2->position.x;
        float top2 = pPositionComponent2->position.y;
        float right2 = pPositionComponent2->position.x + pCollisionComponent2->size.x;
        float bottom2 = pPositionComponent2->position.y + pCollisionComponent2->size.y;

        if (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2) 
        {

            float dx = pPositionComponent->position.x - pPositionComponent2->position.x;
            float dy = pPositionComponent->position.y - pPositionComponent2->position.y;

            float width = (pCollisionComponent->size.x + pCollisionComponent2->size.x) / 2;
            float height = (pCollisionComponent->size.y + pCollisionComponent2->size.y) / 2;

            float crossWidth = width * dy;
            float crossHeight = height * dx;

            if (crossWidth > crossHeight) 
            {
                if (crossWidth > -crossHeight) 
                {
                    return "bottom";
                }
                else 
                {
                    return "left";
                }
            }
            else 
            {
                if (crossWidth > -crossHeight) 
                {
                    return "right";
                }
                else 
                {
                    return "top";
                }
            }
        }

        return "none";
    }
};