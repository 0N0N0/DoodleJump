#pragma once
#include "../Components/SpriteComponent.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"
#include <iostream>
#include "../Components/PositionComponent.h"

struct SpriteWithPosition
{
    SpriteComponent* sprite;
    PositionComponent* position;
};

bool cmpZ(const SpriteWithPosition& a, const SpriteWithPosition& b) {
    return a.position->position.z < b.position->position.z;
}

static void SpriteSystemUpdate(Scene& scene, float deltaTime, sf::RenderWindow& window)
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

    std::vector<SpriteWithPosition> spritesWithPositions;
    for (EntityID ent : SceneView<SpriteComponent, PositionComponent>(scene))
    {
        spritesWithPositions.push_back(SpriteWithPosition{ scene.Get<SpriteComponent>(ent), scene.Get<PositionComponent>(ent) });
    }
    
    std::sort(spritesWithPositions.begin(), spritesWithPositions.end(), cmpZ);
    
    window.clear();
    for (auto& pair : spritesWithPositions)
    {
        pair.sprite->sprite->setPosition(pair.position->position.x - pPositionCamera->position.x, pair.position->position.y - pPositionCamera->position.y);
        window.draw(*pair.sprite->sprite);
    }
    window.display();
}