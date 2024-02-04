#pragma once

#include "Animation.hpp"
#include "Vec2.hpp"

#include <SFML/Graphics.hpp>

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2  pos      = { 0.0f, 0.0f };
	Vec2  velocity = { 0.0f, 0.0f };
	Vec2  scale    = { 1.0f, 1.0f };
	float angle    = 0;

	CTransform()
	{
	}

	CTransform(const Vec2& p)
		: pos(p)
	{
	}

	CTransform(const Vec2& p, const Vec2& v, const Vec2& s, float a)
		: pos(p)
		, velocity(v)
		, scale(s)
		, angle(a)
	{
	}
};

class CInput : public Component
{
public:
	bool up    = false;
	bool down  = false;
	bool left  = false;
	bool right = false;
	bool shoot = false;

	CInput()
	{
	}
};

class CLifespan : public Component
{
};

class CAnimation : public Component
{
public:
	Animation animation;

	CAnimation()
	{
	}

	CAnimation(const Animation& _animation)
		: animation(_animation)
	{
	}
};