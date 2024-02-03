#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
private:
	sf::Sprite m_sprite;

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& texture);

	sf::Sprite& getSprite();
};