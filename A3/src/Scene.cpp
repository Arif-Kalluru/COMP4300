#include "GameEngine.hpp"
#include "Scene.hpp"

Scene::Scene()
{
}

Scene::Scene(GameEngine* game)
	: m_game(game)
{
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::setPaused(bool paused)
{
	m_paused = !m_paused;
}