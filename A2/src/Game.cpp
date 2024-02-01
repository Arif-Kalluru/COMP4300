#include "Game.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#define PI 3.14159265

Game::Game(const std::string& configFilePath)
{
	this->init(configFilePath);
}

void Game::init(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::string token;

	// window config
	int width, height, frameRate;
	bool isFullscreen;
	fin >> token >> width >> height >> frameRate >> isFullscreen;

	if (isFullscreen) {
		m_window.create(sf::VideoMode(width, height), "Assignment 2",
				sf::Style::Fullscreen);
	} else {
		m_window.create(sf::VideoMode(width, height), "Assignment 2");
	}
	m_window.setFramerateLimit(frameRate);

	// font config and set text
	std::string fontName;
	int fontSize, fontR, fontG, fontB;
	fin >> token >> fontName >> fontSize >> fontR >> fontG >> fontB;

	if (!m_font.loadFromFile(fontName)) {
		std::cerr << "Could not load font!\n";
		exit(-1);
	}
	m_text = sf::Text("Score: 0", m_font, fontSize);

	// player config
	PlayerConfig& p = m_playerConfig;  // alias
	fin >> token >> p.SR >> p.CR >> p.S >> p.FR >> p.FG >> p.FB >> p.OR >>
		p.OG >> p.OB >> p.OT >> p.V;

	// enemy config
	EnemyConfig& e = m_enemyConfig;	 // alias
	fin >> token >> e.SR >> e.CR >> e.SMIN >> e.SMAX >> e.OR >> e.OG >>
		e.OB >> e.OT >> e.VMIN >> e.VMAX >> e.L >> e.SI;

	// bullet config
	BulletConfig& b = m_bulletConfig;  // alias
	fin >> token >> b.SR >> b.CR >> b.S >> b.FR >> b.FG >> b.FB >> b.OR >>
		b.OG >> b.OB >> b.OT >> b.V >> b.L;

	this->spawnPlayer();
}

void Game::sMovement()
{
	// player movement
	// reset player movement
	m_player->cTransform->velocity = { 0.0f, 0.0f };

	if (m_player->cInput->up) {
		m_player->cTransform->velocity.y = -m_playerConfig.S;
	} else if (m_player->cInput->down) {
		m_player->cTransform->velocity.y = m_playerConfig.S;
	} else if (m_player->cInput->left) {
		m_player->cTransform->velocity.x = -m_playerConfig.S;
	} else if (m_player->cInput->right) {
		m_player->cTransform->velocity.x = m_playerConfig.S;
	}

	for (const auto e : m_entityManager.getEntities()) {
		e->cTransform->pos += e->cTransform->velocity;
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_running = false;
			return;
		}

		// Keyboard events
		if (event.type == sf::Event::KeyPressed) {
			// Player movement
			switch (event.key.code) {
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			// Exit game
			case sf::Keyboard::Q:
				m_running = false;
				break;
			// Pause game
			case sf::Keyboard::Escape:
				m_paused = !m_paused;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			// Player movement
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default:
				break;
			}
		}

		// Mouse events
		if (event.type == sf::Event::MouseButtonPressed) {
			// Left mouse button shoots a button
			if (event.mouseButton.button == sf::Mouse::Left) {
				Vec2 mousePos(event.mouseButton.x,
					      event.mouseButton.y);
				// Required to check if game is in pause state! Smart asses can
				// pause the game, shoot mutliple bullets then resume, multiple
				// bullets will be spawned!
				if (!m_paused)
					spawnBullet(mousePos);
			}
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (const auto e : m_entityManager.getEntities()) {
		// set position
		e->cShape->circle.setPosition(e->cTransform->pos.x,
					      e->cTransform->pos.y);

		// set rotation
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		// set opacity proportional to it's remaining lifespan
		if (e->cLifespan) {
			int opacity = 255 * (e->cLifespan->remaining) /
				      (e->cLifespan->total);
			// Fill color
			const sf::Color& currFill =
				e->cShape->circle.getFillColor();
			sf::Color fillColor(currFill.r, currFill.g, currFill.b,
					    opacity);
			e->cShape->circle.setFillColor(fillColor);

			// Outline color
			const sf::Color& currOutline =
				e->cShape->circle.getOutlineColor();
			sf::Color outlineColor(currOutline.r, currOutline.g,
					       currOutline.b, opacity);
			e->cShape->circle.setOutlineColor(outlineColor);
		}

		// draw entity's circle
		m_window.draw(e->cShape->circle);
	}

	// Render score
	std::string scoreStr = "Score: " + std::to_string(m_score);
	m_text.setString(scoreStr);
	m_window.draw(m_text);

	// When paused, show instructions and game controls in middle of the screen
	if (m_paused) {
		std::string controlsStr =
			"Press W to move Up\nPress S to move Down\nPress A to move Left\nPress D to move Right\nPress ESC to Play/Pause\nPress Q to Exit\nClick LEFT MOUSE BUTTON to shoot bullet\nClick RIGHT MOUSE BUTTON to use special weapon";
		auto controlsText = sf::Text(controlsStr, m_font,
					     m_text.getCharacterSize());

		// Centering the text
		sf::FloatRect textRect = controlsText.getLocalBounds();
		controlsText.setOrigin(textRect.left + textRect.width / 2.0f,
				       textRect.top + textRect.height / 2.0f);
		const auto& winWidth = m_window.getSize().x;
		const auto& winHeight = m_window.getSize().y;
		controlsText.setPosition(winWidth / 2.0f, winHeight / 2.0f);

		m_window.draw(controlsText);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	// spawn enemy only if certain amount of frames have passed (Spawn Interval)
	if (m_currentFrame - m_lastEnemySpawnFrame < m_enemyConfig.SI)
		return;
	this->spawnEnemy();
}

void Game::sCollision()
{
	auto& playerPos = m_player->cTransform->pos;
	const auto& pCR = m_player->cCollision->collisionRadius;
	const auto& winWidth = m_window.getSize().x;
	const auto& winHeight = m_window.getSize().y;

	// Player bounds check
	// Collision is only checked once per frame, so if else if works
	if (playerPos.y < pCR) {
		// Top collision
		playerPos.y = pCR;
	} else if (playerPos.y + pCR > winHeight) {
		// Bottom collision
		playerPos.y = winHeight - pCR;
	} else if (playerPos.x < pCR) {
		// Left collision
		playerPos.x = pCR;
	} else if (playerPos.x + pCR > winWidth) {
		// Right collision
		playerPos.x = winWidth - pCR;
	}

	// Enemies bounce off boundaries
	for (auto e : m_entityManager.getEntities("enemy")) {
		const auto& enemyPos = e->cTransform->pos;
		auto& enemyVel = e->cTransform->velocity;
		const auto& eCR = e->cCollision->collisionRadius;

		if (enemyPos.y < eCR) {
			// Top collision
			enemyVel.y *= -1.0f;
		} else if (enemyPos.y + eCR > winHeight) {
			// Bottom collision
			enemyVel.y *= -1.0f;
		} else if (enemyPos.x < eCR) {
			// Left collision
			enemyVel.x *= -1.0f;
		} else if (enemyPos.x + eCR > winWidth) {
			// Right collision
			enemyVel.x *= -1.0f;
		}
	}

	// Bullet collision with enemy
	// Spawn smaller enemies when enemy dies
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto e : m_entityManager.getEntities("enemy")) {
			const auto& enemyPos = e->cTransform->pos;
			const auto& eCR = e->cCollision->collisionRadius;
			const auto& bulletPos = b->cTransform->pos;
			const auto& bCR = b->cCollision->collisionRadius;

			// destroy bullet and enemy
			if (bulletPos.dist(enemyPos) < bCR + eCR) {
				e->destroy();
				b->destroy();
				spawnSmallEnemies(e);
				m_score += e->cScore->score;
			}
		}
	}

	// Bullet collision with small enemy
	// Doesn't spawn smaller enemies when it dies
	for (auto b : m_entityManager.getEntities("bullet")) {
		for (auto e : m_entityManager.getEntities("small enemy")) {
			const auto& enemyPos = e->cTransform->pos;
			const auto& eCR = e->cCollision->collisionRadius;
			const auto& bulletPos = b->cTransform->pos;
			const auto& bCR = b->cCollision->collisionRadius;

			// destroy bullet and enemy
			if (bulletPos.dist(enemyPos) < bCR + eCR) {
				e->destroy();
				b->destroy();
				m_score += e->cScore->score;
			}
		}
	}

	// Player collision with enemies
	// If player dies, new spawns at the middle of the screen
	for (const auto e : m_entityManager.getEntities("enemy")) {
		const Vec2& enemyPos = e->cTransform->pos;
		const float& eCR = e->cCollision->collisionRadius;

		// Current player died, spawn a new one
		if (playerPos.dist(enemyPos) < pCR + eCR) {
			m_player->destroy();
			e->destroy();
			spawnPlayer();
			m_score -= e->cScore->score;
			return;
		}
	}

	// Player collision with small enemies
	// If player dies, new spawns at the middle of the screen
	for (const auto e : m_entityManager.getEntities("small enemy")) {
		const Vec2& enemyPos = e->cTransform->pos;
		const float& eCR = e->cCollision->collisionRadius;

		// Current player died, spawn a new one
		if (playerPos.dist(enemyPos) < pCR + eCR) {
			m_player->destroy();
			e->destroy();
			spawnPlayer();
			m_score -= e->cScore->score;
			return;
		}
	}
}

void Game::sLifespan()
{
	for (const auto e : m_entityManager.getEntities()) {
		if (e->cLifespan && e->cLifespan->remaining-- <= 0) {
			e->cLifespan->remaining = 0;
			e->destroy();
		}
	}
}

void Game::spawnPlayer()
{
	PlayerConfig& pC = m_playerConfig;
	auto entity = m_entityManager.addEntity("player");

	// Adding Transform
	// Spawn at the middle of the screen
	float X = m_window.getSize().x / 2.0f;
	float Y = m_window.getSize().y / 2.0f;
	entity->cTransform = std::make_shared<CTransform>(
		Vec2(X, Y), Vec2(0.0f, 0.0f), 0.0f);

	// Adding shape
	sf::Color fillColor(pC.FR, pC.FG, pC.FB);
	sf::Color outlineColor(pC.OR, pC.OG, pC.OB);
	entity->cShape = std::make_shared<CShape>(pC.SR, pC.V, fillColor,
						  outlineColor, pC.OT);

	// Enabling input
	entity->cInput = std::make_shared<CInput>();

	// Enabling collisions
	entity->cCollision = std::make_shared<CCollision>(pC.CR);

	m_player = entity;
}

void Game::spawnEnemy()
{
	EnemyConfig& eC = m_enemyConfig;
	auto entity = m_entityManager.addEntity("enemy");

	// Adding Transform
	// Spawn at random location with shape totally in bounds of screen
	// TODO: try to never spawn enemy within the player's colliding radius
	float X = randomInRange(eC.SR, m_window.getSize().x - eC.SR);
	float Y = randomInRange(eC.SR, m_window.getSize().y - eC.SR);
	// Random velocity
	// Assign random x vel and y vel. Normalize the vector
	// Multiply this vector with random in range [SMIN, SMAX]
	float xVel = randomInRange(-10, 10);
	float yVel = randomInRange(-10, 10);
	Vec2 vel(xVel, yVel);
	vel.normalize();
	vel *= randomInRange(eC.SMIN, eC.SMAX);

	entity->cTransform =
		std::make_shared<CTransform>(Vec2(X, Y), vel, 0.0f);

	// Adding shape
	// Random fill color
	int r = randomInRange(0, 255);
	int g = randomInRange(0, 255);
	int b = randomInRange(0, 255);
	sf::Color fillColor(r, g, b);
	sf::Color outlineColor(eC.OR, eC.OG, eC.OB);

	// Random number of vertices in range [VMIN, VMAX]
	int vertices = randomInRange(eC.VMIN, eC.VMAX);
	entity->cShape = std::make_shared<CShape>(eC.SR, vertices, fillColor,
						  outlineColor, eC.OT);

	// Enabling collisions
	entity->cCollision = std::make_shared<CCollision>(eC.CR);

	// Enemy score, is proportional to number of it's vertices
	entity->cScore = std::make_shared<CScore>(vertices * UNIT_SCORE);

	// Record when the most recent enemy was spawned
	m_lastEnemySpawnFrame = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	int vertices = entity->cShape->circle.getPointCount();
	int step = 360 / vertices;

	EnemyConfig& eC = m_enemyConfig;

	for (int i = 0; i < vertices; i++) {
		auto smallEntity = m_entityManager.addEntity("small enemy");
		// Adding Transform
		Vec2& pos = entity->cTransform->pos;
		// Each small enemy travel outwards at fixed intervals equal to
		// 360/vertices
		float xVel = cos(i * step * PI / 180);
		float yVel = sin(i * step * PI / 180);
		Vec2 vel(xVel, yVel);
		auto speed = entity->cTransform->velocity.length();
		vel *= speed;
		smallEntity->cTransform =
			std::make_shared<CTransform>(pos, vel, 0.0f);

		// Adding shape
		// Small enemies have same color of their parent but half the radius
		auto fillColor = entity->cShape->circle.getFillColor();
		auto outlineColor = entity->cShape->circle.getOutlineColor();

		smallEntity->cShape = std::make_shared<CShape>(
			eC.SR / 2.0f, vertices, fillColor, outlineColor,
			eC.OT / 2.0f);

		// Enabling collisions, small enemies have half the radius
		smallEntity->cCollision =
			std::make_shared<CCollision>(eC.CR / 2);

		// small enemy score is half the original enemy
		smallEntity->cScore =
			std::make_shared<CScore>(vertices * UNIT_SCORE / 2);

		// small enemies have a lifespan
		smallEntity->cLifespan = std::make_shared<CLifespan>(eC.L);
	}
}

void Game::spawnBullet(const Vec2& mousePos)
{
	BulletConfig& bC = m_bulletConfig;
	auto entity = m_entityManager.addEntity("bullet");

	// Adding Transform
	float X = m_player->cTransform->pos.x;
	float Y = m_player->cTransform->pos.y;
	// Finding velocity Vec2
	// Find direction vector from current position of player to mousePos
	// Normalize the vector and multiply with speed of bullet
	Vec2 vel = mousePos - m_player->cTransform->pos;
	vel.normalize();
	vel *= bC.S;

	entity->cTransform =
		std::make_shared<CTransform>(Vec2(X, Y), vel, 0.0f);

	// Adding shape
	sf::Color fillColor(bC.FR, bC.FG, bC.FB);
	sf::Color outlineColor(bC.OR, bC.OG, bC.OB);
	entity->cShape = std::make_shared<CShape>(bC.SR, bC.V, fillColor,
						  outlineColor, bC.OT);

	// Enabling collisions
	entity->cCollision = std::make_shared<CCollision>(bC.CR);

	// Bullets have lifespan
	entity->cLifespan = std::make_shared<CLifespan>(bC.L);
}

void Game::run()
{
	while (m_window.isOpen()) {
		if (!m_running) {
			m_window.close();
			return;
		}

		// If not paused, run these systems
		if (!m_paused) {
			m_entityManager.update();

			this->sEnemySpawner();
			this->sMovement();
			this->sCollision();
			this->sLifespan();
		}

		this->sUserInput();
		this->sRender();

		m_currentFrame++;
	}
}