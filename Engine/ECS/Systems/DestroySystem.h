#pragma once
#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"

#include "../Components/PositionComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/PlatformComponent.h"
#include "../Components/ScoreBarComponent.h"
#include "../Components/MainHeroComponent.h"

#include "CreateSystem.h"

extern float spawnHeight;

static void DestroyAll(Scene& scene)
{
    for (EntityID ent : SceneView<>(scene))
    {
        scene.DestroyEntity(ent);
    }
}

static void DestroySystemUpdate(Scene& scene, Timer& timer)
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


    EntityID scoreBar = -1;
    for (auto scoreB : SceneView<ScoreBarComponent>(scene))
    {
        scoreBar = scoreB;
    }
    if (scoreBar == -1)
    {
        return;
    }
    ScoreBarComponent* pScoreBar = scene.Get<ScoreBarComponent>(scoreBar);

    for (EntityID ent : SceneView<PositionComponent>(scene))
    {
        
        PositionComponent* pPosition = scene.Get<PositionComponent>(ent);
        if (pPositionCamera->position.y + window_height < pPosition->position.y || pPosition->position.y < (spawnHeight > pPositionCamera->position.y ? pPositionCamera->position.y - spawn_margin_height : spawnHeight - spawn_margin_height))
        {
            if (scene.Get<PlatformComponent>(ent))
            {
                CreateSimplePlatform(scene, spawnHeight);
                pScoreBar->platformCount++;
            }

            bool isMainHero = scene.Get<MainHeroComponent>(ent);

            scene.DestroyEntity(ent);

            if (isMainHero)
            {
                spawnHeight = 0;
                DestroyAll(scene);
                CreateGameOver(scene, timer);
                return;
            }
        }
        else if (pPositionCamera->position.x + window_width < pPosition->position.x || pPosition->position.x < pPositionCamera->position.x)
        {
            if (!scene.Get<MainHeroComponent>(ent) && !scene.Get<EnemyComponent>(ent) && !scene.Get<BulletComponent>(ent) && !scene.Get<PlatformLeftRightComponent>(ent))
            {
                if (scene.Get<PlatformComponent>(ent))
                {
                    CreateSimplePlatform(scene, spawnHeight);
                    pScoreBar->platformCount++;
                }

                scene.DestroyEntity(ent);
            }
        }
    }
}