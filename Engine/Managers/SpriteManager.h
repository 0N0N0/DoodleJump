#pragma once
#include <unordered_map>
#include <filesystem>
#include "../Math/Vector3.h"
#include "../../External/include/SFML/Graphics.hpp"

class SpriteManager
{
public:
	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator=(const SpriteManager&) = delete;

	static SpriteManager& GetInstance()
	{
		static SpriteManager instance;
		return instance;
	}

	void LoadSprite(std::string name, std::filesystem::path filepath, sf::Vector2i size)
	{
		sf::Texture texture;
		texture.loadFromFile(filepath.string().c_str());
		auto pairTex = allTextures.insert({ name, texture });

		sf::Sprite* sprite = new sf::Sprite();
		sprite->setTexture(pairTex.first->second);
		sprite->setScale((float)size.x / texture.getSize().x, (float)size.y / texture.getSize().y);
		allSprites.insert({ name, sprite });
	}

	sf::Sprite* GetSprite(std::string name)
	{
		return allSprites.at(name);
	}

	void FreeAllSprites()
	{
		allSprites.clear();
	}

private:
	SpriteManager() {}
	std::unordered_map<std::string, sf::Sprite*> allSprites;
	std::unordered_map<std::string, sf::Texture> allTextures;
};