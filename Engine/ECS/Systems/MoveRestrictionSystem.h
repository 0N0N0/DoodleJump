#pragma once
#include "../Components/Movement.h"
#include "../Components/MoveRestrictionComponent .h"
#include "../Components/PositionComponent.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"
#include <iostream>

static void MoveRestrictionSystemUpdate(Scene& scene, float deltaTime)
{
    for (EntityID ent : SceneView<MoveRestrictionComponent, Movement, PositionComponent>(scene))
    {
        Movement* pMovement = scene.Get<Movement>(ent);

        PositionComponent* pPosition = scene.Get<PositionComponent>(ent);
        MoveRestrictionComponent* pMoveRestriction = scene.Get<MoveRestrictionComponent>(ent);

        Vector3<float> prevPosition = { pPosition->position.x - pMovement->velocity.x * deltaTime, 
                                        pPosition->position.y - pMovement->velocity.y * deltaTime, 
                                        pPosition->position.z - pMovement->velocity.z * deltaTime };

        Vector3<float> oscillationDelta = { pMoveRestriction->position.x - pPosition->position.x,
                                            pMoveRestriction->position.y - pPosition->position.y,
                                            pMoveRestriction->position.z - pPosition->position.z };

        Vector3<float> oscillationDeltaPrev = { pMoveRestriction->position.x - prevPosition.x,
                                    pMoveRestriction->position.y - prevPosition.y,
                                    pMoveRestriction->position.z - prevPosition.z };

        if ((oscillationDelta.x > 0) != (oscillationDeltaPrev.x > 0))
        {
            pMoveRestriction->velocity.x = -pMoveRestriction->velocity.x;
            pMoveRestriction->acceleration.x = -pMoveRestriction->acceleration.x;

            pMovement->velocity.x = pMoveRestriction->velocity.x;
            pMovement->acceleration.x = pMoveRestriction->acceleration.x;
            
            pPosition->position.x = pMoveRestriction->position.x;
        }
        if ((oscillationDelta.y > 0) != (oscillationDeltaPrev.y > 0))
        {
            pMoveRestriction->velocity.y = -pMoveRestriction->velocity.y;
            pMoveRestriction->acceleration.y = -pMoveRestriction->acceleration.y;

            pMovement->velocity.y = pMoveRestriction->velocity.y;
            pMovement->acceleration.y = pMoveRestriction->acceleration.y;

            pPosition->position.y = pMoveRestriction->position.y;
        }
        if ((oscillationDelta.z > 0) != (oscillationDeltaPrev.z > 0))
        {
            pMoveRestriction->velocity.z = -pMoveRestriction->velocity.z;
            pMoveRestriction->acceleration.z = -pMoveRestriction->acceleration.z;

            pMovement->velocity.z = pMoveRestriction->velocity.z;
            pMovement->acceleration.z = pMoveRestriction->acceleration.z;

            pPosition->position.z = pMoveRestriction->position.z;
        }
    }
};