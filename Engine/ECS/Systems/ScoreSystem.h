#pragma once
#include <iostream>
#include <string>

#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"

#include "../Components/PositionComponent.h"
#include "../Components/ScoreBarComponent.h"
#include "../Components/ScoreComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CameraComponent.h"

#include "../../Managers/SpriteManager.h"

struct ScoreWithPosition
{
    SpriteComponent* sprite;
    PositionComponent* position;
    ScoreComponent* score;
};

bool cmpScorePosition(const ScoreWithPosition& a, const ScoreWithPosition& b) {
    return a.score->position < b.score->position;
}

static int ScoreSystemCountDigit(int score)
{
    int digitCount = 0;
    int tempNumber = score;

    do {
        digitCount++;
        tempNumber /= 10;
    } while (tempNumber != 0);

    return digitCount;
}

static std::vector<int> ScoreSystemFillArray(int score, int count)
{
    int tempNumber = score;
    std::vector<int> scoreNum(count, -1);

    for (int i = count - 1; i > -1; i--) {
        scoreNum[i] = tempNumber % 10;
        tempNumber /= 10;
    }

    return scoreNum;
}


static void ScoreSystemUtil(Scene& scene, float deltaTime, std::vector<ScoreWithPosition> scorePosition, std::vector<int> scoreAndScorePlatform, int digitScoreCount, int digitScorePlatformCount)
{
    auto& spriteManager = SpriteManager::GetInstance();

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

    while(digitScoreCount + digitScorePlatformCount > scorePosition.size())
    {
        auto score = scene.NewEntity();
        SpriteComponent* pSprite = scene.Assign<SpriteComponent>(score);
        PositionComponent* pPosition = scene.Assign<PositionComponent>(score);
        ScoreComponent* pScore = scene.Assign<ScoreComponent>(score);
        pSprite->sprite = spriteManager.GetSprite("score0");
        pPosition->position = { 0, 0, -1 };
        pScore->position = scorePosition[scorePosition.size() - 1].score->position + 1;
        scorePosition.push_back(ScoreWithPosition{ pSprite, pPosition, pScore });
    }

    for (int i = 0; i < digitScoreCount; i++)
    {
        scorePosition[i].sprite->sprite = spriteManager.GetSprite("score" + std::to_string(scoreAndScorePlatform[i]));
        scorePosition[i].position->position = { (float)(i * 30),
                                                pPositionCamera->position.y,
                                                5 };
    }

    int k = digitScorePlatformCount;
    for (int i = digitScoreCount; i < digitScoreCount + digitScorePlatformCount; i++)
    {
        scorePosition[i].sprite->sprite = spriteManager.GetSprite("score" + std::to_string(scoreAndScorePlatform[i]));
        scorePosition[i].position->position = { -(float)(k * 30) + window_width,
                                                pPositionCamera->position.y,
                                                5 };
        k--;
    }
}

static void ScoreSystemUpdate(Scene& scene, float deltaTime)
{
    EntityID scoreBar = -1;
    for (auto scoreB : SceneView<ScoreBarComponent, PositionComponent>(scene))
    {
        scoreBar = scoreB;
    }
    if (scoreBar == -1)
    {
        return;
    }

    ScoreBarComponent* pScoreBar = scene.Get<ScoreBarComponent>(scoreBar);

    std::vector<ScoreWithPosition> scorePosition;
    for (EntityID ent : SceneView<SpriteComponent, ScoreComponent, PositionComponent>(scene))
    {
        scorePosition.push_back(ScoreWithPosition{ scene.Get<SpriteComponent>(ent), scene.Get<PositionComponent>(ent), scene.Get<ScoreComponent>(ent) });
    }

    std::sort(scorePosition.begin(), scorePosition.end(), cmpScorePosition);

    int countDigitScore = ScoreSystemCountDigit(pScoreBar->score);
    int countDigitScorePlatform = ScoreSystemCountDigit(pScoreBar->platformCount);

    std::vector<int> score = ScoreSystemFillArray(pScoreBar->score, countDigitScore);
    std::vector<int> scorePlatform = ScoreSystemFillArray(pScoreBar->platformCount, countDigitScorePlatform);

    score.insert(score.end(), scorePlatform.begin(), scorePlatform.end());

    ScoreSystemUtil(scene, deltaTime, scorePosition, score, countDigitScore, countDigitScorePlatform);
}