#include "Game.h"
#include <fstream>
#include <iostream>

#include "utils.h"

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
	m_text.setCharacterSize(fontSize);
	m_text.setFillColor(sf::Color(fontR, fontG, fontB));

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
	fin >> token >> b.SR >> b.CR >> b.FR >> b.FG >> b.FB >> b.OR >> b.OG >>
		b.OB >> b.OT >> b.V >> b.L;

	this->spawnPlayer();
}

void Game::setPaused(bool paused)
{
}

int Game::randomInRange(int lower, int upper)
{
	return lower + rand() % (upper - lower + 1);
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_window.close();
		}

		if (event.type == sf::Event::KeyPressed) {
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

		// draw entity's circle
		m_window.draw(e->cShape->circle);
	}

	m_window.display();
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
	float X = randomInRange(eC.SR, m_window.getSize().x - eC.SR);
	float Y = randomInRange(eC.SR, m_window.getSize().y - eC.SR);
	entity->cTransform = std::make_shared<CTransform>(
		Vec2(X, Y), Vec2(0.0f, 0.0f), 0.0f);

	// Adding shape
	// Random fill color
	int r = this->randomInRange(0, 255);
	int g = this->randomInRange(0, 255);
	int b = this->randomInRange(0, 255);
	sf::Color fillColor(r, g, b);
	sf::Color outlineColor(eC.OR, eC.OG, eC.OB);

	// Random number of vertices in range [VMIN, VMAX]
	int vertices = this->randomInRange(eC.VMIN, eC.VMAX);
	entity->cShape = std::make_shared<CShape>(eC.SR, vertices, fillColor,
						  outlineColor, eC.OT);

	// Enabling collisions
	entity->cCollision = std::make_shared<CCollision>(eC.CR);

	// Enemy score, is proportional to number of it's vertices
	entity->cScore = std::make_shared<CScore>(vertices * UNIT_SCORE);

	// Record when the most recent enemy was spawned
	m_lastEnemySpawnFrame = m_currentFrame;
}

void Game::run()
{
	while (m_window.isOpen()) {
		// TODO: add pause functionality
		if (!m_paused) {
		}

		m_entityManager.update();

		// sEnemySpawner();
		// sMovement();
		// sCollision();
		this->sUserInput();
		this->sRender();

		m_currentFrame++;
	}
}