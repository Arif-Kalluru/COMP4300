#pragma once

#include "Action.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Scene.hpp"

class ScenePlay : public Scene
{
private:
	std::string             m_levelPath;
	EntityManager           m_entityManager;
	std::shared_ptr<Entity> m_player;

	void update()                        override;
	void onEnd()                         override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& levelPath);
	void sMovement();
	void spawnPlayer();

public:
	ScenePlay(GameEngine* gameEngine, const std::string& levelPath);
};