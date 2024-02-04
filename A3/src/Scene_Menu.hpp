#include "GameEngine.hpp"
#include "Scene.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class SceneMenu : public Scene
{
private:
	std::string              m_title;
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text                 m_menuText;
	size_t                   m_selectedMenuIndex = 0;

	void init();

	void onEnd()                         override;
	void update()                        override;
	void sRender()                       override;
	void sDoAction(const Action& action) override;

public:
	SceneMenu(GameEngine* gameEngine);
};