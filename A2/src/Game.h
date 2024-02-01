#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>

#define UNIT_SCORE 10

struct PlayerConfig {
	int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, T;
	float S;
};

struct EnemyConfig {
	int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
	float SMIN, SMAX;
};

struct BulletConfig {
	int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
	float S;
};

class Game {
	sf::RenderWindow m_window;  // window to draw to
	EntityManager m_entityManager;
	sf::Font m_font;
	sf::Text m_text;
	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BulletConfig m_bulletConfig;
	uint32_t m_score = 0;
	uint64_t m_currentFrame = 0;
	uint64_t m_lastEnemySpawnFrame = 0;
	uint64_t m_lastUsedSpecialWeaponFrame = 0;
	bool m_paused = false;
	bool m_running = true;

	std::shared_ptr<Entity> m_player;

	// initialize game state with config file
	void init(const std::string& filePath);

	void sMovement();      // System: Entity position/movement update
	void sUserInput();     // System: User input
	void sLifespan();      // System: Lifespan
	void sRender();	       // System: Render
	void sEnemySpawner();  // System: Spawn enemies
	void sCollision();     // System:  Collision

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& configFilePath);

	void run();
};