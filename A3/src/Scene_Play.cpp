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
    // TODO: remove this testing code
    auto entity = m_entityManager.addEntity("Mario");

    entity->addComponent<CTransform>(Vec2(100, 200), Vec2(0.0f, 0.0f),
                                     Vec2(4.0f, 4.0f), 0); // scale set to 4x16, 4x16
    auto& c = entity->addComponent<CAnimation>(
        m_game->assets().getAnimation("MarioIdleRight"),
        true
    );
    entity->addComponent<CState>().state = "idleRight";
    // TODO: Use player config
    entity->addComponent<CBoundingBox>(Vec2(64, 64));

    m_player = entity;
}

void ScenePlay::update()
{
    m_entityManager.update();
    sMovement();
    // sCollision();
    // sLifespan();
    sAnimation();
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
            auto& playerState = m_player->getComponent<CState>().state;
            // TODO: Ideally jump should be shown until player is in air
            if (playerState == "jumpLeft")
            {
                playerState = "idleLeft";
            }
            else if (playerState == "jumpRight")
            {
                playerState = "idleRight";
            }
        }
        else if (action.name() == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
            m_player->getComponent<CState>().state = "idleLeft";
        }
        else if (action.name() == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
            m_player->getComponent<CState>().state = "idleRight";
        }
    }
}

void ScenePlay::sMovement()
{
    Vec2 playerVelocity(0, 0);

    if (m_player->getComponent<CInput>().up)
    {
        playerVelocity.y -= 5;
        auto& currentState = m_player->getComponent<CState>().state;

        if (currentState == "runRight" || currentState == "idleRight")
        {
            currentState = "jumpRight";
        }
        else if (currentState == "runLeft" || currentState == "idleLeft")
        {
            currentState = "jumpLeft";
        }
    }
    else if (m_player->getComponent<CInput>().right)
    {
        playerVelocity.x += 5;
        m_player->getComponent<CState>().state = "runRight";
    }
    else if (m_player->getComponent<CInput>().left)
    {
        playerVelocity.x -= 5;
        m_player->getComponent<CState>().state = "runLeft";
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

    if (playerState == "runRight")
    {
        setAnimation(m_player, "MarioRunRight", true);
    }
    else if (playerState == "idleRight")
    {
        setAnimation(m_player, "MarioIdleRight", true);
    }
    else if (playerState == "runLeft")
    {
        setAnimation(m_player, "MarioRunLeft", true);
    }
    else if (playerState == "idleLeft")
    {
        setAnimation(m_player, "MarioIdleLeft", true);
    }
    else if (playerState == "jumpLeft")
    {
        setAnimation(m_player, "MarioJumpLeft", true);
    }
    else if (playerState == "jumpRight")
    {
        setAnimation(m_player, "MarioJumpRight", true);
    }

    m_player->getComponent<CAnimation>().animation.update();
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