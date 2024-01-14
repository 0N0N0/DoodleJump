#pragma once

#include <iostream>

#include "../Components/CollisionComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/Movement.h"
#include "../Utils/CheckCollision.h"
#include "../../Scene/SceneView.h"
#include "../../Scene/Scene.h"
#include "AbilitySystem.h"

static void CollisionSystemMainHeroPlatformUpdate(Scene& scene, float deltaTime)
{
    for (EntityID mainHero : SceneView<Movement, CollisionComponent, PositionComponent, MainHeroComponent, JumpingComponent>(scene))
    {
        CollisionComponent* pCollisionComponent = scene.Get<CollisionComponent>(mainHero);
        PositionComponent* pPositionComponent = scene.Get<PositionComponent>(mainHero);
        Movement* pMovement = scene.Get<Movement>(mainHero);
        JumpingComponent* pJumping = scene.Get<JumpingComponent>(mainHero);

        for (EntityID platform : SceneView<CollisionComponent, PositionComponent, PlatformComponent>(scene))
        {
            CollisionComponent* pCollisionComponentPlatform = scene.Get<CollisionComponent>(platform);
            PositionComponent* pPositionComponentPlatform = scene.Get<PositionComponent>(platform);

            if (CheckCollision::CheckCollisionFunc(pPositionComponent, pCollisionComponent, pPositionComponentPlatform, pCollisionComponentPlatform) == "top")
            {
                if (pMovement->velocity.y > 0 && pPositionComponent->position.y + pCollisionComponent->size.y - 10 <= pPositionComponentPlatform->position.y)
                {
                    pMovement->velocity.x = pJumping->velocity.x;
                    pMovement->velocity.y = pJumping->velocity.y;
                    pMovement->velocity.z = pJumping->velocity.z;
                }
            }
        }
    }
}

static void CollisionSystemMainHeroEnemyUpdate(Scene& scene, float deltaTime)
{
    for (EntityID mainHero : SceneView<Movement, CollisionComponent, PositionComponent, MainHeroComponent, JumpingComponent>(scene))
    {
        CollisionComponent* pCollisionComponent = scene.Get<CollisionComponent>(mainHero);
        PositionComponent* pPositionComponent = scene.Get<PositionComponent>(mainHero);
        Movement* pMovement = scene.Get<Movement>(mainHero);
        JumpingComponent* pJumping = scene.Get<JumpingComponent>(mainHero);

        for (EntityID enemy : SceneView<CollisionComponent, PositionComponent, EnemyComponent>(scene))
        {
            CollisionComponent* pCollisionComponent2 = scene.Get<CollisionComponent>(enemy);
            PositionComponent* pPositionComponent2 = scene.Get<PositionComponent>(enemy);

            std::string collisionResult = CheckCollision::CheckCollisionFunc(pPositionComponent, pCollisionComponent, pPositionComponent2, pCollisionComponent2);
            if (collisionResult == "top")
            {
                if (pMovement->velocity.y > 0)
                {
                    pMovement->velocity.x = pJumping->velocity.x;
                    pMovement->velocity.y = pJumping->velocity.y;
                    pMovement->velocity.z = pJumping->velocity.z;
                    scene.DestroyEntity(enemy);
                }
            }
            else if (collisionResult != "none")
            {   
                CreateFallingMainHero(scene, pPositionComponent->position);

                scene.DestroyEntity(mainHero);
            }
        }
    }
}

static void CollisionSystemBulletEnemyUpdate(Scene& scene, float deltaTime)
{
    for (EntityID bullet : SceneView<Movement, CollisionComponent, PositionComponent, BulletComponent>(scene))
    {
        CollisionComponent* pCollisionBullet = scene.Get<CollisionComponent>(bullet);
        PositionComponent* pPositionBullet = scene.Get<PositionComponent>(bullet);

        for (EntityID enemy: SceneView<Movement, CollisionComponent, PositionComponent, EnemyComponent>(scene))
        {
            CollisionComponent* pCollisionEnemy = scene.Get<CollisionComponent>(enemy);
            PositionComponent* pPositionEnemy = scene.Get<PositionComponent>(enemy);

            std::string collisionResult = CheckCollision::CheckCollisionFunc(pPositionBullet, pCollisionBullet, pPositionEnemy, pCollisionEnemy);
            if (collisionResult != "none")
            {
                scene.DestroyEntity(enemy);
                scene.DestroyEntity(bullet);
            }
        }
    }
}

static void CollisionSystemMainHeroAbilities(Scene& scene, Timer& timer)
{
    for (EntityID mainHero : SceneView<CollisionComponent, PositionComponent, MainHeroComponent, AbilityComponent>(scene))
    {
        CollisionComponent* pCollisionMainHero = scene.Get<CollisionComponent>(mainHero);
        PositionComponent* pPositionMainHero = scene.Get<PositionComponent>(mainHero);
        AbilityComponent* pAbilityMainHero = scene.Get<AbilityComponent>(mainHero);

        for (EntityID ability : SceneView<CollisionComponent, PositionComponent, AbilityComponent>(scene))
        {
            if (!scene.Get<MainHeroComponent>(ability))
            {
                CollisionComponent* pCollisionAbility = scene.Get<CollisionComponent>(ability);
                PositionComponent* pPositionAbility = scene.Get<PositionComponent>(ability);
                AbilityComponent* pAbility = scene.Get<AbilityComponent>(ability);
                SpriteComponent* pSpriteAbility = scene.Get<SpriteComponent>(ability);

                std::string collisionResult = CheckCollision::CheckCollisionFunc(pPositionMainHero, pCollisionMainHero, pPositionAbility, pCollisionAbility);
                if (collisionResult != "none")
                {
                    auto& spriteManager = SpriteManager::GetInstance();
                    
                    if (pSpriteAbility->sprite == spriteManager.GetSprite("autoShooting"))
                    {
                        static EventTimer* currentAbilityEvent = nullptr;
                        pAbilityMainHero->autoShooting = true;

                        float stateNormaliseTimeForAbility = 20.0f;

                        auto autoShootLambda = [pAbilityMainHero]()
                        {
                            pAbilityMainHero->autoShooting = false;
                            currentAbilityEvent = nullptr;
                        };

                        if (!currentAbilityEvent)
                        {
                            currentAbilityEvent = timer.AddEventTimer(stateNormaliseTimeForAbility, autoShootLambda);
                        }
                        else
                        {
                            currentAbilityEvent->time = stateNormaliseTimeForAbility;
                        }
                    }
                    else if (pSpriteAbility->sprite == spriteManager.GetSprite("spring"))
                    {
                        pAbilityMainHero->jump = true;
                    }

                    scene.DestroyEntity(ability);
                }
            }
        }
    }
}