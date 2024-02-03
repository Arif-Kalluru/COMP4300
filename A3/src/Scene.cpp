#include "GameEngine.hpp"
#include "Scene.hpp"

Scene::Scene()
{
}

Scene::Scene(GameEngine* game)
	: m_game(game)
{
}