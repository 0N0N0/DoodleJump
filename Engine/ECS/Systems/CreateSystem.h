#pragma once
#include "../../Scene/Scene.h"
#include "../../Scene/SceneView.h"

#include "../Components/SpriteComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/Movement.h"
#include "../Components/BulletComponent.h"
#include "../Components/MoveRestrictionComponent .h"
#include "../Components/CameraComponent.h"
#include "../Components/PlatformComponent.h"
#include "../Components/PlatformLeftRightComponent.h"
#include "../Components/PlatformUpDownComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/AbilityComponent.h"
#include "../Components/NoseComponent.h"

#include "../../Managers/SpriteManager.h"
#include <random>
#include <chrono>
#include <iostream>

static void CreateMain(Scene& scene)
{
	auto& spriteManager = SpriteManager::GetInstance();
	SpriteComponent* pSprite;
	PositionComponent* pPosition;
	Movement* pMovement;
	CollisionComponent* pCollision;
	MainHeroComponent* pMainHero;
	PlatformComponent* pPlatform;
	EnemyComponent* pEnemy;
	MoveRestrictionComponent* pMoveRestriction;
	JumpingComponent* pJumping;
	CameraComponent* pCamera;
	BackgroundComponent* pBackground;
	ScoreBarComponent* pScoreBar;
	ScoreComponent* pScore;
	BulletComponent* pBullet;
	AbilityComponent* pAbility;

	auto camera = scene.NewEntity();
	pCamera = scene.Assign<CameraComponent>(camera);
	pPosition = scene.Assign<PositionComponent>(camera);
	pPosition->position = { 0, 0, 0 };
	pPosition->name = "camera";

	auto background = scene.NewEntity();
	pSprite = scene.Assign<SpriteComponent>(background);
	pPosition = scene.Assign<PositionComponent>(background);
	pBackground = scene.Assign<BackgroundComponent>(background);
	pSprite->sprite = spriteManager.GetSprite("background");
	pPosition->position = { 0, 0, 0 };
	pPosition->name = "background";

	auto scoreBar = scene.NewEntity();
	pSprite = scene.Assign<SpriteComponent>(scoreBar);
	pPosition = scene.Assign<PositionComponent>(scoreBar);
	pScoreBar = scene.Assign<ScoreBarComponent>(scoreBar);
	pSprite->sprite = spriteManager.GetSprite("scoreBar");
	pPosition->position = { 0, 0, 4 };
	pPosition->name = "scoreBar";
	pScoreBar->score = 0;
	pScoreBar->platformCount = 0;
	pScoreBar->maxPosition = window_height / 2;

	auto score = scene.NewEntity();
	pSprite = scene.Assign<SpriteComponent>(score);
	pPosition = scene.Assign<PositionComponent>(score);
	pScore = scene.Assign<ScoreComponent>(score);
	pSprite->sprite = spriteManager.GetSprite("score0");
	pPosition->position = { 0, 0, -1 };
	pPosition->name = "score";
	pScore->position = 0;

	auto doodle = scene.NewEntity();
	pMainHero = scene.Assign<MainHeroComponent>(doodle);
	pSprite = scene.Assign<SpriteComponent>(doodle);
	pPosition = scene.Assign<PositionComponent>(doodle);
	pMovement = scene.Assign<Movement>(doodle);
	pCollision = scene.Assign<CollisionComponent>(doodle);
	pJumping = scene.Assign<JumpingComponent>(doodle);
	pAbility = scene.Assign<AbilityComponent>(doodle);
	pSprite->sprite = spriteManager.GetSprite("doodleLeft");
	pPosition->position = { (float)window_width/2, (float)window_height/ 2, 3 };
	pPosition->name = "doodle";
	pMovement->velocity = { 0, -500, 0 };
	pMovement->acceleration = { 0, 500, 0 };
	pCollision->size = { 50, 60, 0 };
	pJumping->velocity = pMovement->velocity;

	float platformHeight = window_height - spawn_margin_height;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<> distWidth(0, window_width - 40);
	for (int i = 0; i < 20; i++)
	{
		auto platform = scene.NewEntity();
		pSprite = scene.Assign<SpriteComponent>(platform);
		pPosition = scene.Assign<PositionComponent>(platform);
		pCollision = scene.Assign<CollisionComponent>(platform);
		pPlatform = scene.Assign<PlatformComponent>(platform);
		pSprite->sprite = spriteManager.GetSprite("platform");
		pPosition->position = { (float)distWidth(rng), platformHeight, 1 };
		pPosition->name = "platform";
		pCollision->size = { 30, 13, 0 };

		platformHeight -= spawn_margin_height;
	}

	auto monster = scene.NewEntity();
	pSprite = scene.Assign<SpriteComponent>(monster);
	pPosition = scene.Assign<PositionComponent>(monster);
	pCollision = scene.Assign<CollisionComponent>(monster);
	pMovement = scene.Assign<Movement>(monster);
	pEnemy = scene.Assign<EnemyComponent>(monster);
	pMoveRestriction = scene.Assign<MoveRestrictionComponent>(monster);
	pSprite->sprite = spriteManager.GetSprite("enemy1");
	pPosition->position = { 400, 550, 3 };
	pPosition->name = "monster";
	pCollision->size = { 80, 50, 0 };
	pMovement->velocity = { 200, 0, 0 };
	pMovement->acceleration = { -200, 0, 0 };
	pMoveRestriction->position = pPosition->position;
	pMoveRestriction->acceleration = pMovement->acceleration;
	pMoveRestriction->velocity = pMovement->velocity;
}

static void CreateFallingMainHero(Scene& scene, Vector3<float> position)
{
	auto& spriteManager = SpriteManager::GetInstance();
	auto doodle = scene.NewEntity();
	MainHeroComponent* pMainHero = scene.Assign<MainHeroComponent>(doodle);
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(doodle);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(doodle);
	Movement* pMovement = scene.Assign<Movement>(doodle);
	pSprite->sprite = spriteManager.GetSprite("doodleStars1");
	pPosition->position = position;
	pPosition->name = "stars";
	pMovement->velocity = { 0, 500, 0 };
	pMovement->acceleration = { 0, 0, 0 };
}

static void CreateBullet(Scene& scene, Vector3<float> position, Vector3<float> normalisedVelocity)
{
	auto& spriteManager = SpriteManager::GetInstance();
	auto bullet = scene.NewEntity();
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(bullet);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(bullet);
	CollisionComponent* pCollision = scene.Assign<CollisionComponent>(bullet);
	Movement* pMovement = scene.Assign<Movement>(bullet);
	BulletComponent* pBullet = scene.Assign<BulletComponent>(bullet);
	pSprite->sprite = spriteManager.GetSprite("bullet");
	pCollision->size = { 16, 16, 0 };
	pPosition->position = { position.x - pCollision->size.x / 2, position.y - pCollision->size.y / 2, position.z };
	pPosition->name = "bullet";
	pMovement->velocity = { 500.0f * normalisedVelocity.x, 500.0f * normalisedVelocity.y, 0};
	pMovement->acceleration = { 0, 0, 0 };
}

static void CreateEnemy(Scene& scene, Vector3<float> position)
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

	auto& spriteManager = SpriteManager::GetInstance();
	auto enemy = scene.NewEntity();
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(enemy);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(enemy);
	CollisionComponent* pCollision = scene.Assign<CollisionComponent>(enemy);
	Movement* pMovement = scene.Assign<Movement>(enemy);
	EnemyComponent* pEnemy = scene.Assign<EnemyComponent>(enemy);
	MoveRestrictionComponent* pMoveRestriction = scene.Assign<MoveRestrictionComponent>(enemy);
	pCollision->size = { 80, 50, 0 };

	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<> enemyRand(1, 3);
	pSprite->sprite = spriteManager.GetSprite("enemy" + std::to_string(enemyRand(rng)));

	
	PositionComponent* pPositionCamera = scene.Get<PositionComponent>(camera);

	pPosition->position = { position.x - pCollision->size.x/4, position.y - pCollision->size.y, position.z };
	pPosition->name = "monster";

	std::uniform_int_distribution<> vel(0, 200);
	std::uniform_int_distribution<> acc(0, 200);

	pMovement->velocity = { (float)vel(rng), 0, 0};
	pMovement->acceleration = { -(float)acc(rng), 0, 0};

	pMoveRestriction->position = pPosition->position;
	pMoveRestriction->acceleration = pMovement->acceleration;
	pMoveRestriction->velocity = pMovement->velocity;
}

static void CreateLeftRightPlatform(Scene& scene, Vector3<float> position)
{
	auto& spriteManager = SpriteManager::GetInstance();
	auto platformLeftRight = scene.NewEntity();
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(platformLeftRight);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(platformLeftRight);
	CollisionComponent* pCollision = scene.Assign<CollisionComponent>(platformLeftRight);
	Movement* pMovement = scene.Assign<Movement>(platformLeftRight);
	PlatformComponent* pPlatform = scene.Assign<PlatformComponent>(platformLeftRight);
	PlatformLeftRightComponent* pPlatformLeftRight = scene.Assign<PlatformLeftRightComponent>(platformLeftRight);
	pSprite->sprite = spriteManager.GetSprite("platformLeftRight");
	pCollision->size = { 30, 13, 0 };
	pPosition->position = position;
	pPosition->name = "LRplatform";
	pMovement->acceleration = { 0 , 0 ,0 };
	pMovement->velocity = {100, 0, 0};
}

static void CreateUpDownPlatform(Scene& scene, Vector3<float> position)
{
	auto& spriteManager = SpriteManager::GetInstance();
	auto platformUpDown = scene.NewEntity();
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(platformUpDown);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(platformUpDown);
	CollisionComponent* pCollision = scene.Assign<CollisionComponent>(platformUpDown);
	Movement* pMovement = scene.Assign<Movement>(platformUpDown);
	PlatformComponent* pPlatform = scene.Assign<PlatformComponent>(platformUpDown);
	PlatformUpDownComponent* pPlatformUpDown = scene.Assign<PlatformUpDownComponent>(platformUpDown);
	pSprite->sprite = spriteManager.GetSprite("platformUpDown");
	pCollision->size = { 30, 13, 0 };
	pPosition->position = position;
	pPosition->name = "UDplatform";
	pMovement->acceleration = { 0 , 0 ,0 };
	pMovement->velocity = { 0, 50, 0 };
	pPlatformUpDown->amplitude = 50;
	pPlatformUpDown->startPositionHeight = position.y;
}


static void CreateAbility(Scene& scene, Vector3<float> platformPosition)
{
	auto& spriteManager = SpriteManager::GetInstance();
	auto ability = scene.NewEntity();
	SpriteComponent* pSprite = scene.Assign<SpriteComponent>(ability);
	PositionComponent* pPosition = scene.Assign<PositionComponent>(ability);
	CollisionComponent* pCollision = scene.Assign<CollisionComponent>(ability);
	AbilityComponent* pAbility = scene.Assign<AbilityComponent>(ability);

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);

	auto rndResult = dist(rng);

	if (rndResult == 0)
	{
		pSprite->sprite = spriteManager.GetSprite("autoShooting");
		pCollision->size = { 30, 30, 0 };
		pPosition->position = { platformPosition.x + pCollision->size.x / 4, platformPosition.y - pCollision->size.y, platformPosition.z };
		pAbility->autoShooting = false;
	}
	else if (rndResult == 1)
	{
		pSprite->sprite = spriteManager.GetSprite("spring");
		pCollision->size = { 15, 30, 0 };
		pPosition->position = { platformPosition.x + pCollision->size.x / 2, platformPosition.y - pCollision->size.y, platformPosition.z };
		pAbility->jump = false;
	}
	pPosition->name = "ability";
}

static void CreateSimplePlatform(Scene& scene, float& spawnHeight)
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

	auto scoreBar = *SceneView<ScoreBarComponent>(scene).begin();
	ScoreBarComponent* pScoreBar = scene.Get<ScoreBarComponent>(scoreBar);

	static float spawn_height_with_difficulty = spawn_margin_height;
	spawn_height_with_difficulty = spawn_margin_height + (int)pScoreBar->score / 1000;

	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<> distWidth(0, window_width - 40);

	PositionComponent* pPositionCamera = scene.Get<PositionComponent>(camera);
	std::uniform_int_distribution<> distHight(0, spawn_height_with_difficulty / 4);

	if (spawnHeight > pPositionCamera->position.y)
	{
		spawnHeight = pPositionCamera->position.y - fmod(pPositionCamera->position.y, spawn_height_with_difficulty);
	}

	Vector3<float> platformPosition = { (float)distWidth(rng), spawnHeight - (float)distHight(rng), 1 };
	spawnHeight -= spawn_height_with_difficulty;

	static int abilityHeight = 0;
	std::uniform_int_distribution<> platformChanceDist(16, 31);

	auto chanceAnotherPlatformSpawn = platformChanceDist(rng);
	if (chanceAnotherPlatformSpawn == 30)
	{
		CreateLeftRightPlatform(scene, platformPosition);
	}
	else if (chanceAnotherPlatformSpawn == 31)
	{
		CreateUpDownPlatform(scene, platformPosition);
	}
	else
	{
		auto& spriteManager = SpriteManager::GetInstance();
		auto platform = scene.NewEntity();
		SpriteComponent* pSprite = scene.Assign<SpriteComponent>(platform);
		PositionComponent* pPosition = scene.Assign<PositionComponent>(platform);
		CollisionComponent* pCollision = scene.Assign<CollisionComponent>(platform);
		PlatformComponent* pPlatform = scene.Assign<PlatformComponent>(platform);
		pSprite->sprite = spriteManager.GetSprite("platform");
		pCollision->size = { 30, 13, 0 };

		pPosition->position = platformPosition;
		pPosition->name = "platform";

		std::uniform_int_distribution<> spawEnemyRandom(0, 15);
		auto spawnEnemyChance = spawEnemyRandom(rng);
		if ((pScoreBar->score - pScoreBar->score % 1000) / 1000 > abilityHeight)
		{
			CreateAbility(scene, platformPosition);
			abilityHeight++;
		}
		else if (spawnEnemyChance == 0)
		{
			CreateEnemy(scene, { platformPosition.x, platformPosition.y, 2 });
		}
	}
}

static void CreateGameOver(Scene& scene, Timer& timer)
{
	auto& spriteManager = SpriteManager::GetInstance();

	auto camera = scene.NewEntity();
	auto pCamera = scene.Assign<CameraComponent>(camera);
	auto pPosition = scene.Assign<PositionComponent>(camera);
	pPosition->position = { 0, 0, 0 };

	auto background = scene.NewEntity();
	auto pSprite = scene.Assign<SpriteComponent>(background);
	pPosition = scene.Assign<PositionComponent>(background);
	pSprite->sprite = spriteManager.GetSprite("background");
	pPosition->position = { 0, 0, 1 };

	auto raceOver = scene.NewEntity();
	pSprite = scene.Assign<SpriteComponent>(raceOver);
	pPosition = scene.Assign<PositionComponent>(raceOver);
	pSprite->sprite = spriteManager.GetSprite("raceOver");
	pPosition->position = { (float)window_width / 2 - 50, (float)window_height / 2, 2 };

	timer.AddEventTimer(3.0f, [&scene, camera, background, raceOver]() 
		{
			scene.DestroyEntity(camera);
			scene.DestroyEntity(background);
			scene.DestroyEntity(raceOver);
			CreateMain(scene);
		});
}

static void CreateNose(Scene& scene, PositionComponent position, Movement movement)
{
	auto doodleNose = scene.NewEntity();
	auto pSprite = scene.Assign<SpriteComponent>(doodleNose);
	auto pPosition = scene.Assign<PositionComponent>(doodleNose);
	auto pMovement = scene.Assign<Movement>(doodleNose);
	auto pNose = scene.Assign<NoseComponent>(doodleNose);

	auto& spriteManager = SpriteManager::GetInstance();

	pSprite->sprite = spriteManager.GetSprite("doodleNose");
	*pPosition = position;
	pPosition->name = "nose";
	*pMovement = movement;
}