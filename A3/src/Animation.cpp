#include "Animation.hpp"
#include "Vec2.hpp"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& texture)
	: Animation(name, texture, 1, 0)
{
}

Animation::Animation(const std::string& name, const sf::Texture& texture,
                     size_t keyframesCount, size_t duration)
	: m_name(name)
	, m_sprite(texture)
	, m_keyframesCount(keyframesCount)
	, m_currentKeyframe(0)
	, m_duration(duration)
{
	m_size = Vec2(
				(float)texture.getSize().x / keyframesCount,
				(float)texture.getSize().y
			 );
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(
		sf::IntRect(m_currentKeyframe * m_size.x, 0, m_size.x, m_size.y)
	);
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

void Animation::update()
{
	m_gameFrames++;
	m_currentKeyframe = (m_gameFrames / m_duration) % m_keyframesCount;
	const auto rectange = sf::IntRect(
						  	m_currentKeyframe * m_size.x, 0, m_size.x, m_size.y
						  );
	m_sprite.setTextureRect(rectange);
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

const std::string& Animation::getName() const
{
	return m_name;
}

bool Animation::hasEnded() const
{
	// At least one keyframe has been played but current keyframe is still the 0th,
	// hence it's looped around, which implies at least one loop of animation has ended
	return (m_gameFrames > m_duration && m_currentKeyframe == 0) ? true : false;
}