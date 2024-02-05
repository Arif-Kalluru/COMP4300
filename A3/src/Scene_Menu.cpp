#include "Action.hpp"
#include "Scene_Menu.hpp"
#include "Scene_Play.hpp"

SceneMenu::SceneMenu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void SceneMenu::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::Up, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Down, "DOWN");
	registerAction(sf::Keyboard::Enter, "SELECT");
	registerAction(sf::Keyboard::Q, "QUIT");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_title = "Super Mario Clone";
	m_menuStrings.push_back("Level 1");
	m_menuStrings.push_back("Level 2");
	m_menuStrings.push_back("Level 3");

	m_levelPaths.push_back("level1.txt");
	m_levelPaths.push_back("level2.txt");
	m_levelPaths.push_back("level3.txt");

	m_menuText.setFont(m_game->assets().getFont("Joystix"));
	m_menuText.setCharacterSize(64);
}

void SceneMenu::update()
{
	sRender();
}

void SceneMenu::onEnd()
{
	m_game->quit();
}

void SceneMenu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "SELECT")
		{
			m_game->changeScene(
				"PLAY",
				std::make_shared<ScenePlay>(
					m_game,
					m_levelPaths[m_selectedMenuIndex]
				)
			);
		}
		else if (action.name() == "UP")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex > 0)
								  ? --m_selectedMenuIndex
			                      : m_menuStrings.size() - 1;
		}
		else if (action.name() == "DOWN")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex + 1) %
								       m_menuStrings.size();
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}

// System: Render
void SceneMenu::sRender()
{
	auto& window = m_game->window();

	window.clear(sf::Color::Blue); // Blue color background

	// Render title text
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::White);
	// Center the title text
	const float posX = window.getSize().x / 2.0f -
	                       m_menuText.getLocalBounds().width / 2.0f;
	const float posY = 10;
	m_menuText.setPosition(posX, posY);
	window.draw(m_menuText);

	// Render menu text
	for (int stringIndex = 0; stringIndex < m_menuStrings.size(); stringIndex++)
	{
		const float posX = 20;
		const float posY = 150 + stringIndex * 100;
		m_menuText.setString(m_menuStrings[stringIndex]);
		m_menuText.setPosition(posX, posY);

		if (m_selectedMenuIndex == stringIndex)
		{
			m_menuText.setFillColor(sf::Color::Black); // Black colour current selected option in the menu
		}
		else
		{
			m_menuText.setFillColor(sf::Color::White); // White colour not selected text
		}

		window.draw(m_menuText);
	}

	window.display();
}