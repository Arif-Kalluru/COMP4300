#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "GameEngine.hpp"

#include <memory>

class GameEngine; // Resolve circular dependency

class Scene
{
protected:
	GameEngine*   m_game          = nullptr;
	uint64_t      m_currentFrame  = 0;
	EntityManager m_entityManager;

	// TODO: remove this player; it's testing code
	std::shared_ptr<Entity> m_player;

public:
	Scene();
	Scene(GameEngine* game);

	virtual void init()    = 0;
	virtual void update()  = 0;
	virtual void sRender() = 0;
};