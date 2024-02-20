#pragma once

#include "Vec2.hpp"

#include <SFML/Graphics.hpp>

class Animation
{
private:
	std::string m_name            = "none";
	size_t      m_keyframesCount  = 1;        // total number of keyframes of animation
	size_t      m_duration        = 1;        // duration of this animation (in game frames)
	size_t      m_currentKeyframe = 0;        // the current keyframe of animation being played
	size_t      m_gameFrames      = 0;        // number of game frames that have passed
	Vec2        m_size            = { 1, 1 }; // dimensions of the animation keyframe
	sf::Sprite  m_sprite;

public:
	Animation();
	Animation(const std::string& name, const sf::Texture& texture);
	Animation(const std::string& name, const sf::Texture& texture,
	      	  size_t keyframesCount, size_t duration);

	sf::Sprite&        getSprite();
	const std::string& getName()   const;
	const Vec2&        getSize()   const;

	void update();
	bool hasEnded() const;
};