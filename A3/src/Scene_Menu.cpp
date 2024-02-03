#include "Scene_Menu.hpp"

// TODO: Remove this testing code
#include <iostream>
#include "EntityManager.hpp"
#include "Animation.hpp"

SceneMenu::SceneMenu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	this->init();
}

void SceneMenu::init()
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

void SceneMenu::update()
{
	this->sRender();
}

// System: Render
void SceneMenu::sRender()
{
	// TODO: remove this testing code
	auto& window = m_game->window();

	auto& pos = m_player->getComponent<CTransform>().pos;
	auto& scale = m_player->getComponent<CTransform>().scale;
	auto& sprite = m_player->getComponent<CAnimation>().animation.getSprite();
	sprite.setPosition(pos.x, pos.y);
	sprite.setScale(scale.x, scale.y);

	window.clear();
	window.draw(sprite);
	window.display();
}