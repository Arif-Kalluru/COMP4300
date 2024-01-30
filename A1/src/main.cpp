#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

// Font Properties
struct FontProps {
	std::string name;
	sf::Font gameFont;
	uint16_t size;
	uint16_t r;
	uint16_t g;
	uint16_t b;
};

// Game config object
struct Game {
	FontProps font;
	int windowWidth;
	int windowHeight;
};

class Entity {
public:
	std::shared_ptr<sf::Shape> shape;
	std::shared_ptr<sf::Text> text;
	float sx;
	float sy;
};

void loadConfigFile(const std::string& filename, Game& game, std::vector<std::shared_ptr<Entity>>& entities);

int main(int argc, char* argv[])
{
	Game game;
	std::vector<std::shared_ptr<Entity>> entities;
	loadConfigFile("bin/config.txt", game, entities);
	sf::RenderWindow window(sf::VideoMode(game.windowWidth, game.windowHeight), "SFML works!");
	// 60 FPS
	window.setFramerateLimit(60);

	if (!game.font.gameFont.loadFromFile(game.font.name)) {
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	sf::Text sampleText("Press x to reverse velocities.", game.font.gameFont, game.font.size);
	sampleText.setPosition(0, 0);

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				std::cout << "Key pressed with code = " << event.key.code << "\n";

				// Move in opposite direction
				if (event.key.code == sf::Keyboard::X) {
					for (const auto& e : entities) {
						e->sx *= -1.0f;
						e->sy *= -1.0f;
					}
				}
			}
		}

		// Update position
		for (auto& e : entities) {
			float shapeWidth = e->shape->getLocalBounds().width;
			float shapeHeight = e->shape->getLocalBounds().height;
			float shapeX = e->shape->getPosition().x;
			float shapeY = e->shape->getPosition().y;
			float textWidth = e->text->getLocalBounds().width;
			float textHeight = e->text->getLocalBounds().height;

			// Check for collisions
			// Left or right collision
			if (shapeX < 0 || (shapeX + shapeWidth > game.windowWidth)) {
				e->sx *= -1.0f;
			}

			// Top or bottom collision
			if (shapeY < 0 || (shapeY + shapeHeight > game.windowHeight)) {
				e->sy *= -1.0f;
			}

			e->shape->setPosition(e->shape->getPosition().x + e->sx, e->shape->getPosition().y + e->sy);
			// center the text with the entity
			e->text->setPosition(shapeX +  (shapeWidth / 2) - (textWidth / 2),
				shapeY + (shapeHeight / 2) - (textHeight / 2));
		}

		// Rendering
		window.clear();
		for (const auto& e : entities) {
			window.draw(*e->shape);
			window.draw(*e->text);
		}
		window.draw(sampleText);
		window.display();
	}

	return 0;
}

// Set game object & load entities.
void loadConfigFile(const std::string& filename, Game& game, std::vector<std::shared_ptr<Entity>>& entities)
{
	std::ifstream fin(filename);
	std::string token;

	fin >> token >> game.windowWidth >> game.windowHeight;
	fin >> token >> game.font.name >> game.font.size >> game.font.r >> game.font.b >> game.font.g;

	while (fin >> token) {
		auto e = std::make_shared<Entity>();

		if (token == "Circle") {
			auto pCircle = std::make_shared<sf::CircleShape>();
			// Name
			std::string name;
			fin >> name;
			e->text = std::make_shared<sf::Text>(name, game.font.gameFont, game.font.size);
			e->text->setFillColor(sf::Color(game.font.r, game.font.g, game.font.b));

			// Initial co-ordinates
			int x, y;
			fin >> x >> y;
			pCircle->setPosition(x, y);

			// Speed
			fin >> e->sx >> e->sy;

			// Color
			int r, g, b;
			fin >> r >> g >> b;
			pCircle->setFillColor(sf::Color(r, g, b));

			// Radius
			float radius;
			fin >> radius;
			pCircle->setRadius(radius);

			e->shape = pCircle;
			entities.push_back(e);
		} else if (token == "Rectangle") {
			auto pRect = std::make_shared<sf::RectangleShape>();
			// Name
			std::string name;
			fin >> name;
			e->text = std::make_shared<sf::Text>(name, game.font.gameFont, game.font.size);
			e->text->setFillColor(sf::Color(game.font.r, game.font.g, game.font.b));

			// Initial co-ordinates
			int x, y;
			fin >> x >> y;
			pRect->setPosition(x, y);

			// Speed
			fin >> e->sx >> e->sy;

			// Color
			int r, g, b;
			fin >> r >> g >> b;
			pRect->setFillColor(sf::Color(r, g, b));

			// Size
			float width, height;
			fin >> width >> height;
			pRect->setSize(sf::Vector2f(width, height));

			e->shape = pRect;
			entities.push_back(e);
		}
	}
}