#pragma once

#include "../../Scene/SceneView.h"
#include "../../Scene/Scene.h"

#include "../../Managers/SpriteManager.h"

#include "../../Utils/Timer.h"

#include "../Components/Movement.h"
#include "../Components/PositionComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/MainHeroComponent.h"
#include "../Components/CameraComponent.h"
#include "CreateSystem.h"

static void AbilitySystemAutoShooting(Scene& scene, float deltaTime)
{
    static int bulletSpawnTimer = 100;
    EntityID camera = -1;
    for (auto cam : SceneView<CameraComponent, PositionComponent>(scene))
    {
        camera = cam;
    }
    if (camera == -1)
    {
        return;
    }
    PositionComponent* pPositionCamera = scene.Get<PositionComponent>(camera);

    if (bulletSpawnTimer <= 0)
    {
        for (EntityID mainHero : SceneView<MainHeroComponent, PositionComponent, CollisionComponent, SpriteComponent>(scene))
        {
            PositionComponent* pPositionMainHero = scene.Get<PositionComponent>(mainHero);
            CollisionComponent* pCollisionMainHero = scene.Get<CollisionComponent>(mainHero);
            SpriteComponent* pSpriteMainHero = scene.Get<SpriteComponent>(mainHero);

            for (EntityID enemy : SceneView<Movement, CollisionComponent, PositionComponent, EnemyComponent>(scene))
            {
                CollisionComponent* pCollisionEnemy = scene.Get<CollisionComponent>(enemy);
                PositionComponent* pPositionEnemy = scene.Get<PositionComponent>(enemy);

                if (pPositionEnemy->position.y > pPositionCamera->position.y)
                {
                    Vector3<float> centeredPosition = { pPositionMainHero->position.x + pCollisionMainHero->size.x / 2 , pPositionMainHero->position.y + pCollisionMainHero->size.y / 2, 2 };

                    Vector3<float> doodleToEnemy = { pPositionEnemy->position.x - pPositionMainHero->position.x, pPositionEnemy->position.y - pPositionMainHero->position.y };
                    float diagonal = sqrt(doodleToEnemy.x * doodleToEnemy.x + doodleToEnemy.y * doodleToEnemy.y);

                    CreateBullet(scene, centeredPosition, { doodleToEnemy.x / diagonal, doodleToEnemy.y / diagonal, 0 });
                    bulletSpawnTimer = 100;
                }
            }
        }
    }
    bulletSpawnTimer -= deltaTime;
}

static void AbilitySystemJumping(Scene& scene, float deltaTime)
{
    EntityID camera = -1;
    for (auto cam : SceneView<CameraComponent, PositionComponent>(scene))
    {
        camera = cam;
    }
    if (camera == -1)
    {
        return;
    }
    PositionComponent* pPositionCamera = scene.Get<PositionComponent>(camera);

    
    for (EntityID mainHero : SceneView<MainHeroComponent, PositionComponent, CollisionComponent, SpriteComponent>(scene))
    {
        Movement* pMovement = scene.Get<Movement>(mainHero);

        pMovement->velocity.y = -1000;
    }
}

static void AbilitySystemUpdate(Scene& scene, float deltaTime)
{
    for (EntityID ability : SceneView<CollisionComponent, PositionComponent, AbilityComponent, MainHeroComponent>(scene))
    {
        AbilityComponent* pAbility = scene.Get<AbilityComponent>(ability);

        if (pAbility->autoShooting == true)
        {
            AbilitySystemAutoShooting(scene, deltaTime);
        }
        if (pAbility->jump == true)
        {
            AbilitySystemJumping(scene, deltaTime);
            pAbility->jump = false;
        }
    }
}