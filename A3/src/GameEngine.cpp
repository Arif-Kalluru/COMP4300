#include "GameEngine.hpp"
#include "Scene_Menu.hpp"

#include <fstream>
#include <SFML/Graphics.hpp>

GameEngine::GameEngine(const std::string& filePath)
{
	this->init(filePath);
}

void GameEngine::init(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::string token;
	int width, height, frameRate;
	bool fullScreenMode;

	while (fin.good())
	{
		fin >> token >> width >> height >> frameRate >> fullScreenMode;
	}

	if (fullScreenMode)
	{
		m_window.create(sf::VideoMode(width, height), "Custom Game Engine",
		                sf::Style::Fullscreen);
	}
	else
	{
		m_window.create(sf::VideoMode(width, height), "Custom Game Engine");
	}

	m_window.setFramerateLimit(frameRate);

	// Load assets
	m_assets.loadFromFile("bin/assets.txt");

	// Change scene to menu
	changeScene("MENU", std::make_shared<SceneMenu>(this));
}

void GameEngine::update()
{
	this->sUserInput();
	this->currentScene()->update();
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_SceneMap[m_currentScene];
}

// System: User input
void GameEngine::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->quit();
		}
	}
}

bool GameEngine::isRunning() const
{
	return m_running && m_window.isOpen();
}

const Assets& GameEngine::assets() const
{
	return m_assets;
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while (this->isRunning())
	{
		this->update();
	}

	m_window.close();
}

void GameEngine::quit()
{
	m_running = false;
}

void GameEngine::changeScene(const std::string& sceneName,
                             std::shared_ptr<Scene> scene)
{
	m_SceneMap[sceneName] = scene;
	m_currentScene = sceneName;
}