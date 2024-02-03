#include "GameEngine.hpp"
#include "Scene.hpp"

class SceneMenu : public Scene
{
public:
	SceneMenu(GameEngine* gameEngine);

	void init()     override;
	void update()   override;
	void sRender()  override;
};