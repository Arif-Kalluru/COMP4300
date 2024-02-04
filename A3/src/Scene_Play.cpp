#include "Action.hpp"
#include "Scene_Menu.hpp"
#include "Scene_Play.hpp"
#include "Vec2.hpp"

#include <iostream>

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	this->init(levelPath);
}

void ScenePlay::init(const std::string& levelPath)
{
	this->registerAction(sf::Keyboard::W, "JUMP");
	this->registerAction(sf::Keyboard::Up, "JUMP");
	this->registerAction(sf::Keyboard::Escape, "EXIT");
	this->registerAction(sf::Keyboard::P, "PAUSE");

	this->loadLevel(levelPath);
}

void ScenePlay::loadLevel(const std::string& levelPath)
{
	// Reset entity manager on every new level
	m_entityManager = EntityManager();

	// Read level from level file
	// Read player config from player config file

	this->spawnPlayer();
}

void ScenePlay::spawnPlayer()
{
	// TODO: remove this testing code
	auto entity = m_entityManager.addEntity("Mario");

	entity->addComponent<CTransform>(Vec2(100, 200), Vec2(0.0f, 0.0f),
	                                 Vec2(3.0f, 3.0f), 0);
	auto& c = entity->addComponent<CAnimation>(
		m_game->assets().getAnimation("MarioIdleRight")
	);

	m_player = entity;
}

void ScenePlay::update()
{
	m_entityManager.update();
	this->sMovement();
	this->sRender();
}

void ScenePlay::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void ScenePlay::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "JUMP")
		{
			m_player->getComponent<CInput>().up = true;
		}
		else if (action.name() == "EXIT")
		{
			this->onEnd();
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "JUMP")
		{
			m_player->getComponent<CInput>().up = false;
		}
	}
}

void ScenePlay::sMovement()
{
	Vec2 playerVelocity(0, 0);

	if (m_player->getComponent<CInput>().up)
	{
		playerVelocity.y -= 3;
	}

	m_player->getComponent<CTransform>().velocity = playerVelocity;

	for (auto e : m_entityManager.getEntities())
	{
		// if (e->hasComponent<CGravity>())
		// {
		// 	e->getComponent<CTransform>().velocity.y +=
		// 		e->getComponent<CGravity>().gravity;
		// }

		e->getComponent<CTransform>().pos +=
			e->getComponent<CTransform>().velocity;
	}
}

void ScenePlay::sRender()
{
	// TODO: remove this testing code
	auto& window = m_game->window();
	window.clear();

	for (auto e : m_entityManager.getEntities())
	{
		auto& pos = e->getComponent<CTransform>().pos;
		auto& scale = e->getComponent<CTransform>().scale;
		auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
		sprite.setPosition(pos.x, pos.y);
		sprite.setScale(scale.x, scale.y);

		window.draw(sprite);
	}

	window.display();
}