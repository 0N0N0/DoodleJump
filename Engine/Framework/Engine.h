#pragma once
#include "../Math/Vector3.h"
#include "../Scene/Scene.h"
#include "../Utils/Timer.h"
#include "../Managers/SpriteManager.h"
#include "../Managers/InputHandler.h"

#include "../ECS/Components/MainHeroComponent.h"
#include "../ECS/Components/PlatformComponent.h"
#include "../ECS/Components/EnemyComponent.h"
#include "../ECS/Components/JumpingComponent.h"
#include "../ECS/Components/CameraComponent.h"
#include "../ECS/Components/BackgroundComponent.h"
#include "../ECS/Components/ScoreBarComponent.h"
#include "../ECS/Components/ScoreComponent.h"
#include "../ECS/Components/BulletComponent.h"

#include "../ECS/Systems/MoveSystem.h"
#include "../ECS/Systems/SpriteSystem.h"
#include "../ECS/Systems/CollisionSystem.h"
#include "../ECS/Systems/MoveRestrictionSystem.h"
#include "../ECS/Systems/ScoreSystem.h"
#include "../ECS/Systems/DestroySystem.h"

#include "../../External/include/SFML/Audio.hpp"
#include "../../External/include/SFML/Graphics.hpp"
#include "../../External/include/SFML/Network.hpp"
#include "../../External/include/SFML/System.hpp"
#include "../../External/include/SFML/Window.hpp"

int window_width = 0;
int window_height = 0;
int spawn_margin_height = 0;
float spawnHeight = 0;

class MyFramework {

public:

	virtual void PreInit(int& width, int& height)
	{
		spawn_margin_height = 40;
		width = window_width;
		height = window_height;
	}

	virtual bool Init() 
	{
		auto& spriteManager = SpriteManager::GetInstance();
		spriteManager.LoadSprite("background", "../../data/hop-bck.png", sf::Vector2i{ window_width, window_height });
		spriteManager.LoadSprite("scoreBar", "../../data/top-score-bar.png", sf::Vector2i{ window_width, 50 });

		spriteManager.LoadSprite("score0", "../../data/score-0.png", sf::Vector2i{ 25, 31 });
		spriteManager.LoadSprite("score1", "../../data/score-1.png", sf::Vector2i{ 9, 31 });
		spriteManager.LoadSprite("score2", "../../data/score-2.png", sf::Vector2i{ 29, 31 });
		spriteManager.LoadSprite("score3", "../../data/score-3.png", sf::Vector2i{ 25, 31 });
		spriteManager.LoadSprite("score4", "../../data/score-4.png", sf::Vector2i{ 21, 31 });
		spriteManager.LoadSprite("score5", "../../data/score-5.png", sf::Vector2i{ 27, 31 });
		spriteManager.LoadSprite("score6", "../../data/score-6.png", sf::Vector2i{ 26, 31 });
		spriteManager.LoadSprite("score7", "../../data/score-7.png", sf::Vector2i{ 26, 31 });
		spriteManager.LoadSprite("score8", "../../data/score-8.png", sf::Vector2i{ 24, 31 });
		spriteManager.LoadSprite("score9", "../../data/score-9.png", sf::Vector2i{ 22, 31 });

		spriteManager.LoadSprite("doodleLeft", "../../data/jungle-left@2x.png", sf::Vector2i{ 60, 60 });
		spriteManager.LoadSprite("doodleRight", "../../data/jungle-right@2x.png", sf::Vector2i{ 60, 60 });
		spriteManager.LoadSprite("doodleShoot", "../../data/jungle-puca@2x.png", sf::Vector2i{ 60, 60 });
		spriteManager.LoadSprite("doodleNose", "../../data/jungle-njuska@2x.png", sf::Vector2i{ 15, 40 });
		spriteManager.LoadSprite("doodleStars1", "../../data/jungle-left-stars1@2x.png", sf::Vector2i{ 60, 60 });
		spriteManager.LoadSprite("doodleStars2", "../../data/jungle-left-stars2@2x.png", sf::Vector2i{ 60, 60 });
		spriteManager.LoadSprite("doodleStars3", "../../data/jungle-left-stars3@2x.png", sf::Vector2i{ 60, 60 });

		spriteManager.LoadSprite("platformUpDown", "../../data/up-down-platform.png", sf::Vector2i{ 40, 13 });
		spriteManager.LoadSprite("platformLeftRight", "../../data/left-right-platform.png", sf::Vector2i{ 40, 13 });
		spriteManager.LoadSprite("platform", "../../data/simple-platform.png", sf::Vector2i{ 40, 13 });

		spriteManager.LoadSprite("enemy1", "../../data/game-tiles-jungle-monster1@2x.png", sf::Vector2i{ 80, 50 });
		spriteManager.LoadSprite("enemy2", "../../data/game-tiles-jungle-monster2@2x.png", sf::Vector2i{ 80, 50 });
		spriteManager.LoadSprite("enemy3", "../../data/game-tiles-jungle-monster3@2x.png", sf::Vector2i{ 80, 50 });

		spriteManager.LoadSprite("bullet", "../../data/bubble@2x.png", sf::Vector2i{ 16, 16 });
		spriteManager.LoadSprite("autoShooting", "../../data/auto-shooting@2x.png", sf::Vector2i{ 30, 30 });
		spriteManager.LoadSprite("spring", "../../data/spring.png", sf::Vector2i{ 15, 30 });

		spriteManager.LoadSprite("raceOver", "../../data/race-over.png", sf::Vector2i{ 100, 30 });

		CreateMain(scene);
		
		return true;
	}

	virtual void Close() 
	{
		auto& spriteManager = SpriteManager::GetInstance();
		spriteManager.FreeAllSprites();
	}

	virtual bool Tick(sf::RenderWindow& window)
	{
		timer.Start(60);

		AbilitySystemUpdate(scene, timer.GetDeltaTime());

		MoveSystemUpdate(scene, timer.GetDeltaTime());
		MoveSystemEnemyAndBulletUpdate(scene, timer.GetDeltaTime());
		MoveSystemInputBulletUpdate(scene, timer, timer.GetDeltaTime());
		MoveSystemInputHeroUpdate(scene, timer.GetDeltaTime());
		MoveSystemPlatformLeftRightUpdate(scene);
		MoveSystemPlatformUpDownUpdate(scene);
		MoveSystemFallingMainHeroUpdate(scene);

		CollisionSystemMainHeroPlatformUpdate(scene, timer.GetDeltaTime());
		CollisionSystemMainHeroEnemyUpdate(scene, timer.GetDeltaTime());
		CollisionSystemBulletEnemyUpdate(scene, timer.GetDeltaTime());
		CollisionSystemMainHeroAbilities(scene, timer);

		MoveRestrictionSystemUpdate(scene, timer.GetDeltaTime());

		ScoreSystemUpdate(scene, timer.GetDeltaTime());
		SpriteSystemUpdate(scene, timer.GetDeltaTime(), window);

		DestroySystemUpdate(scene, timer);

		timer.Update();
		timer.Wait();
		
		return false;
	}

	virtual void onMouseMove(int x, int y) 
	{
		auto& inputHandler = InputHandler::GetInstance();

		inputHandler.m_mouse.x = x;
		inputHandler.m_mouse.y = y;

	}

	virtual void onMouseButtonClick(const sf::Event& event)
	{
		if (event.mouseButton.button == sf::Mouse::Left) 
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_mouse.Left = true;
		}
	}

	virtual void onMouseButtonRelease(const sf::Event& event)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_mouse.Left = false;
		}
	}

	virtual void onKeyPressed(const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::Right)
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_keys.Right = true;
		}
		else if (event.key.code == sf::Keyboard::Left)
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_keys.Left = true;
		}
	}

	virtual void onKeyReleased(const sf::Event& event)
	{
		if (event.key.code == sf::Keyboard::Right)
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_keys.Right = false;
		}
		else if (event.key.code == sf::Keyboard::Left)
		{
			auto& inputHandler = InputHandler::GetInstance();
			inputHandler.m_keys.Left = false;
		}
	}

	virtual void run(sf::RenderWindow& window)
	{
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event)) 
			{
				switch (event.type)
				{
					case sf::Event::Closed:
					{
						Close();
						window.close();
						return;
					}
					case sf::Event::MouseMoved:
					{
						onMouseMove(event.mouseMove.x, event.mouseMove.y);
						break;
					}
					case sf::Event::MouseButtonPressed:
					{
						onMouseButtonClick(event);
						break;
					}
					case sf::Event::MouseButtonReleased:
					{
						onMouseButtonRelease(event);
						break;
					}
					case sf::Event::KeyPressed:
					{
						onKeyPressed(event);
						break;
					}
					case sf::Event::KeyReleased:
					{
						onKeyReleased(event);
						break;
					}
				}
			}

			Tick(window);
		}
	}

private:
	Timer timer;
	Scene scene;
};