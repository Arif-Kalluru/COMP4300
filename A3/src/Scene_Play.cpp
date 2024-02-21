#include "Action.hpp"
#include "Scene_Menu.hpp"
#include "Scene_Play.hpp"
#include "Vec2.hpp"

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
{
    init(levelPath);
}

void ScenePlay::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::Up, "JUMP");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Right, "RIGHT");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::Left, "LEFT");
    registerAction(sf::Keyboard::Escape, "EXIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing textures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing collision boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing Grid

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Arial"));

    loadLevel(levelPath);
}

void ScenePlay::loadLevel(const std::string& levelPath)
{
    // Reset entity manager on every new level
    m_entityManager = EntityManager();

    // TODO: Read level from level file
    // TODO: Read player config from player config file

    spawnPlayer();
}

void ScenePlay::spawnPlayer()
{
    // TODO: Use player config
    auto entity = m_entityManager.addEntity("Mario");

    entity->addComponent<CTransform>(Vec2(100, 200), Vec2(0.0f, 0.0f),
                                     Vec2(4.0f, 4.0f), 0); // scale set to 4x16, 4x16
    auto& c = entity->addComponent<CAnimation>(
        m_game->assets().getAnimation("MarioIdle"),
        true
    );
    entity->addComponent<CState>("idle");
    entity->addComponent<CBoundingBox>(Vec2(64, 64));
    entity->addComponent<CGravity>(0.75);

    m_player = entity;
}

void ScenePlay::update()
{
    if (!m_paused)
    {
        m_entityManager.update();
        sMovement();
        sCollision();
        // sLifespan();
        sAnimation();
    }

    sRender();

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
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "EXIT")
        {
            onEnd();
        }
        else if (action.name() == "PAUSE")
        {
            setPaused(m_paused);
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            m_drawGrid = !m_drawGrid;
        }
        else if (action.name() == "TOGGLE_TEXTURE")
        {
            m_drawTextures = !m_drawTextures;
        }
        else if (action.name() == "TOGGLE_COLLISION")
        {
            m_drawCollision = !m_drawCollision;
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "JUMP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
    }
}

void ScenePlay::sMovement()
{
    Vec2 playerVelocity(0, 0);

    auto& input = m_player->getComponent<CInput>();
    auto& playerState = m_player->getComponent<CState>().state;
    auto& playerTransform = m_player->getComponent<CTransform>();

    // Player movement
    if (playerState != "jump")
    {
        if (input.up)
        {
            if (playerState == "run" || playerState == "idle")
            {
                playerState = "jump";
            }

            // When you go into a jump from running state, preserve the x direction velocity
            playerVelocity.x = playerTransform.velocity.x;

            playerVelocity.y -= 15;
        }
        else if (input.right)
        {
            playerVelocity.x += 5;
            playerState = "run";
            playerTransform.scale.x = abs(playerTransform.scale.x);
        }
        else if (input.left)
        {
            playerVelocity.x -= 5;
            playerState = "run";
            playerTransform.scale.x = -abs(playerTransform.scale.x);
        }
        else if (!input.right && playerState == "run")
        {
            playerState = "idle";
            playerTransform.scale.x = abs(playerTransform.scale.x);
        }
        else if (!input.left && playerState == "run")
        {
            playerState = "idle";
            playerTransform.scale.x = -abs(playerTransform.scale.x);
        }

        m_player->getComponent<CTransform>().velocity = playerVelocity;
    }

    // Update velocities
    for (auto e : m_entityManager.getEntities())
    {
        // Gravity
        if (e->hasComponent<CGravity>())
        {
        	e->getComponent<CTransform>().velocity.y +=
        		e->getComponent<CGravity>().gravity;
        }

        e->getComponent<CTransform>().pos +=
            e->getComponent<CTransform>().velocity;
    }
}

/* Helper function */
void ScenePlay::setAnimation(std::shared_ptr<Entity> entity,
                    const std::string& animationName, bool repeat)
{
    // TODO: Maybe this function should be outside the class
    if (entity->getComponent<CAnimation>().animation.getName() != animationName)
    {
        entity->getComponent<CAnimation>() =
            CAnimation(m_game->assets().getAnimation(animationName), repeat);
    }
}

void ScenePlay::sAnimation()
{
    const auto& playerState = m_player->getComponent<CState>().state;

    if (playerState == "run")
    {
        setAnimation(m_player, "MarioRun", true);
    }
    else if (playerState == "idle")
    {
        setAnimation(m_player, "MarioIdle", true);
    }
    else if (playerState == "jump")
    {
        setAnimation(m_player, "MarioJump", true);
    }

    m_player->getComponent<CAnimation>().animation.update();
}

void ScenePlay::sCollision()
{
    // TODO: Remove this testing code. Imaginary platform
    auto& playerTransform = m_player->getComponent<CTransform>();
    auto& playerState = m_player->getComponent<CState>().state;

    if (playerTransform.pos.y > 200) // collision with platform
    {
        playerTransform.pos.y = 200;
        if (playerState == "jump")
        {
            playerState = "idle";
            playerTransform.velocity = { 0.0f, 0.0f };
        }
    }
}

void ScenePlay::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
    m_game->window().draw(line, 2, sf::Lines);
}

void ScenePlay::sRender()
{
    // TODO: remove this testing code
    auto& window = m_game->window();

    if (!m_paused)
    {
        // color the background darker when paused
        window.clear(sf::Color(100, 100, 255));
    }
    else
    {
        window.clear(sf::Color(50, 50, 150));
    }

    // Set the viewport of the window to be centered on the player if it's far enough right

    if (m_drawGrid)
    {
    }

    // draw bounding box
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x, box.size.y));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                window.draw(rect);
            }
        }
    }

    // draw all entity textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto& pos = e->getComponent<CTransform>().pos;
            auto& scale = e->getComponent<CTransform>().scale;
            auto& sprite = e->getComponent<CAnimation>().animation.getSprite();
            sprite.setPosition(pos.x, pos.y);
            sprite.setScale(scale.x, scale.y);

            window.draw(sprite);
        }
    }

    window.display();
}