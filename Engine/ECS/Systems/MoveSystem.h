#pragma once
#include <math.h>
#include <iostream>

#include "../Systems/CreateSystem.h"

#include "../../Managers/InputHandler.h"
#include "../../Managers/SpriteManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"

#include "../../Utils/Timer.h"

#include "../Components/PositionComponent.h"
#include "../Components/Movement.h"
#include "../Components/MainHeroComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BackgroundComponent.h"
#include "../Components/ScoreBarComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/PlatformComponent.h"
#include "../Components/ScoreComponent.h"
#include "../Components/PlatformLeftRightComponent.h"
#include "../Components/PlatformUpDownComponent.h"

#define M_PI 3.14159265358979323846

static void MoveSystemUpdate(Scene& scene, float deltaTime)
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

    for (EntityID ent : SceneView<Movement, PositionComponent>(scene))
    {
        PositionComponent* pPosition = scene.Get<PositionComponent>(ent);
        Movement* pMovement = scene.Get<Movement>(ent);

        float prevPosition = pPosition->position.y;

        pPosition->position.x += pMovement->velocity.x * deltaTime;
        pPosition->position.y += pMovement->velocity.y * deltaTime;
        pPosition->position.z += pMovement->velocity.z * deltaTime;

        pMovement->velocity.x += pMovement->acceleration.x * deltaTime;
        pMovement->velocity.y += pMovement->acceleration.y * deltaTime;
        pMovement->velocity.z += pMovement->acceleration.z * deltaTime;

        if (scene.Get<MainHeroComponent>(ent))
        {
            auto scoreBar = *SceneView<ScoreBarComponent, PositionComponent>(scene).begin();
            ScoreBarComponent* pScoreBar = scene.Get<ScoreBarComponent>(scoreBar);
            
            if (pScoreBar->maxPosition > pPosition->position.y)
            {
                auto posDelta = prevPosition - pPosition->position.y;
                pScoreBar->score += posDelta > 0 ? posDelta : 0;
                pScoreBar->maxPosition = pPosition->position.y < pScoreBar->maxPosition ? pPosition->position.y : pScoreBar->maxPosition;
            }
        }
    }
}

static void MoveSystemEnemyAndBulletUpdate(Scene& scene, float deltaTime)
{
    for (EntityID enemy : SceneView<PositionComponent, EnemyComponent, CollisionComponent>(scene))
    {
        PositionComponent* pPosition = scene.Get<PositionComponent>(enemy);
        CollisionComponent* pCollision = scene.Get<CollisionComponent>(enemy);

        if (pPosition->position.x > window_width + pCollision->size.x)
        {
            pPosition->position.x = -pCollision->size.x;
        }
        else if (pPosition->position.x < -pCollision->size.x)
        {
            pPosition->position.x = window_width + pCollision->size.x;
        }
    }

    for (EntityID bullet : SceneView<PositionComponent, BulletComponent, CollisionComponent>(scene))
    {
        PositionComponent* pPosition = scene.Get<PositionComponent>(bullet);
        CollisionComponent* pCollision = scene.Get<CollisionComponent>(bullet);

        if (pPosition->position.x > window_width + pCollision->size.x)
        {
            pPosition->position.x = - pCollision->size.x;
        }
        else if (pPosition->position.x < - pCollision->size.x)
        {
            pPosition->position.x = window_width + pCollision->size.x;
        }
    }
}

static void MoveSystemInputHeroUpdate(Scene& scene, float deltaTime)
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

    auto& inputHandler = InputHandler::GetInstance();
    Vector3<float> move = { (inputHandler.m_keys.Left * -200) + (inputHandler.m_keys.Right * 200), 0, 0 };

    for (EntityID ent : SceneView<MainHeroComponent, PositionComponent, SpriteComponent, CollisionComponent>(scene))
    {
        auto& spriteManager = SpriteManager::GetInstance();
        PositionComponent* pPositionComponent = scene.Get<PositionComponent>(ent);
        SpriteComponent* pSprite = scene.Get<SpriteComponent>(ent);
        CollisionComponent* pCollision = scene.Get<CollisionComponent>(ent);

        pPositionComponent->position.x += move.x * deltaTime;
        pPositionComponent->position.y += move.y * deltaTime;
        pPositionComponent->position.z += move.z * deltaTime;

        if (pPositionComponent->position.x > window_width + pCollision->size.x)
        {
            pPositionComponent->position.x = -pCollision->size.x;
        }
        else if (pPositionComponent->position.x < -pCollision->size.x)
        {
            pPositionComponent->position.x = window_width + pCollision->size.x;
        }

        auto background = *SceneView<BackgroundComponent, PositionComponent>(scene).begin();
        PositionComponent* pPositionBackground = scene.Get<PositionComponent>(background);

        if (pPositionComponent->position.y - pPositionCamera->position.y < window_height / 2)
        {
            auto scoreBar = *SceneView<ScoreBarComponent, PositionComponent>(scene).begin();
            PositionComponent* pPositionScoreBar = scene.Get<PositionComponent>(scoreBar);

            pPositionCamera->position.y -= (pPositionCamera->position.y + window_height / 2) - pPositionComponent->position.y;
            pPositionBackground->position.y = pPositionCamera->position.y;
            pPositionScoreBar->position.y = pPositionCamera->position.y;

            for (EntityID score : SceneView<ScoreComponent, PositionComponent>(scene))
            {
                PositionComponent* pPositionScore = scene.Get<PositionComponent>(score);
                pPositionScore->position.y = pPositionScoreBar->position.y;
            }
        }

        for (EntityID nose : SceneView<NoseComponent, PositionComponent>(scene))
        {
            PositionComponent* pPositionNose = scene.Get<PositionComponent>(nose);
            SpriteComponent* pSpriteNose = scene.Get<SpriteComponent>(nose);

            pPositionNose->position.x = pPositionComponent->position.x + pSprite->sprite->getLocalBounds().width / 6;
            pPositionNose->position.y = pPositionComponent->position.y + pSprite->sprite->getLocalBounds().height / 12;
        }
    }
}

float angleBetweenVectors(const sf::Vector2f& v1, const sf::Vector2f& v2) {
    float dotProduct = v1.x * v2.x + v1.y * v2.y;
    float magnitudeProduct = std::sqrt(v1.x * v1.x + v1.y * v1.y) * std::sqrt(v2.x * v2.x + v2.y * v2.y);

    if (magnitudeProduct == 0.0f) {
        return 0.0f;
    }

    float angle = std::acos(dotProduct / magnitudeProduct) * (180.0f / static_cast<float>(M_PI));

    return angle;
}

static void MoveSystemInputBulletUpdate(Scene& scene, Timer& timer, float deltaTime)
{
    auto& inputHandler = InputHandler::GetInstance();
    static EventTimer* currentEvent = nullptr;
    float stateNormaliseTime = 0.5f;

    if (inputHandler.m_mouse.Left)
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
        for (EntityID ent : SceneView<MainHeroComponent, PositionComponent, CollisionComponent, SpriteComponent>(scene))
        {
            PositionComponent* pPosition = scene.Get<PositionComponent>(ent);
            CollisionComponent* pCollision = scene.Get<CollisionComponent>(ent);
            SpriteComponent* pSprite = scene.Get<SpriteComponent>(ent);
            Movement* pMovement = scene.Get<Movement>(ent);

            Vector3<float> centeredPosition = { pPosition->position.x + pCollision->size.x / 2 , pPosition->position.y + pCollision->size.y / 2, 2 };

            auto& spriteManager = SpriteManager::GetInstance();
            auto currentSprite = pSprite->sprite;
            pSprite->sprite = spriteManager.GetSprite("doodleShoot");

            Vector3<float> doodleToMouse = { inputHandler.m_mouse.x + pPositionCamera->position.x - centeredPosition.x, inputHandler.m_mouse.y + pPositionCamera->position.y - centeredPosition.y };
            float diagonal = sqrt(doodleToMouse.x * doodleToMouse.x + doodleToMouse.y * doodleToMouse.y);

            CreateBullet(scene, centeredPosition, { doodleToMouse.x / diagonal, doodleToMouse.y / diagonal, 0 });

            inputHandler.m_mouse.Left = false;

            if (!currentEvent)
            {
                CreateNose(scene, *pPosition, *pMovement);

                for (EntityID nose : SceneView<NoseComponent>(scene))
                {
                    SpriteComponent* pSpriteNose = scene.Get<SpriteComponent>(nose);
                }
            
                auto shootLambda = [&scene, ent, currentSprite]()
                {  
                    for (EntityID mainHero : SceneView<MainHeroComponent, SpriteComponent>(scene))
                    {
                        scene.Get<SpriteComponent>(mainHero)->sprite = currentSprite;
                    }
                    for (EntityID nose : SceneView<NoseComponent>(scene))
                    {
                        scene.DestroyEntity(nose);
                    }
                    currentEvent = nullptr;
                };
  
                currentEvent = timer.AddEventTimer(stateNormaliseTime, shootLambda);
            }
            else
            {
                currentEvent->time = stateNormaliseTime;
            }
        }
    }
}

static void MoveSystemPlatformLeftRightUpdate(Scene& scene)
{
    for (EntityID platform : SceneView<PositionComponent, PlatformLeftRightComponent, CollisionComponent, Movement>(scene))
    {
        PositionComponent* pPosition = scene.Get<PositionComponent>(platform);
        CollisionComponent* pCollision = scene.Get<CollisionComponent>(platform);
        Movement* pMovement = scene.Get<Movement>(platform);

        if (pPosition->position.x <= 0)
        {
            pMovement->velocity.x = -pMovement->velocity.x;
        }
        else if (pPosition->position.x + pCollision->size.x + 10 >= window_width)
        {
            pMovement->velocity.x = -pMovement->velocity.x;
        }
    }
}

static void MoveSystemPlatformUpDownUpdate(Scene& scene)
{
    for (EntityID platform : SceneView<PositionComponent, PlatformUpDownComponent, CollisionComponent, Movement>(scene))
    {
        PositionComponent* pPosition = scene.Get<PositionComponent>(platform);
        CollisionComponent* pCollision = scene.Get<CollisionComponent>(platform);
        Movement* pMovement = scene.Get<Movement>(platform);
        PlatformUpDownComponent* pPlatformUpDown = scene.Get<PlatformUpDownComponent>(platform);

        if (pPosition->position.y <= pPlatformUpDown->startPositionHeight - pPlatformUpDown->amplitude)
        {
            pMovement->velocity.y = -pMovement->velocity.y;
        }
        else if (pPosition->position.y >= pPlatformUpDown->amplitude + pPlatformUpDown->startPositionHeight)
        {
            pMovement->velocity.y = -pMovement->velocity.y;
        }
    }
}

static void MoveSystemFallingMainHeroUpdate(Scene& scene)
{
    for (EntityID mainHero: SceneView<PositionComponent, MainHeroComponent, Movement, SpriteComponent>(scene))
    {
        if (!scene.Get<CollisionComponent>(mainHero))
        {
            SpriteComponent* pSprite = scene.Get<SpriteComponent>(mainHero);
            PositionComponent* pPosition = scene.Get<PositionComponent>(mainHero);
            auto& spriteManager = SpriteManager::GetInstance();

            int i = 1;
            if (pPosition->position.y < 0)
            {
                i = -1;
            }
            int spriteNumber = (int)(pPosition->position.y * i) % 3 + 1;
            pSprite->sprite = spriteManager.GetSprite("doodleStars" + std::to_string(spriteNumber));

            for (EntityID nose : SceneView<NoseComponent>(scene))
            {
                scene.DestroyEntity(nose);
            }
        }
    }
}