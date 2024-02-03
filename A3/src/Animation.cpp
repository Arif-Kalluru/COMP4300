#include <Animation.hpp>

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& texture)
{
	m_sprite.setTexture(texture);
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}