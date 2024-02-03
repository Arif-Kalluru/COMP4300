#include "Assets.hpp"

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

Assets::Assets()
{
}

bool Assets::addTexture(const std::string& textureName, const std::string& path,
                        bool smooth)
{
	sf::Texture texture;

	if (!texture.loadFromFile(path))
	{
		std::cerr << "Failed to load texture: " << textureName << " " <<
		          path << std::endl;
		return false;
	}

	texture.setSmooth(smooth);
	m_textureMap[textureName] = texture;
	return true;
}

bool Assets::addTexture(const std::string& textureName, const std::string& path,
                        int posX, int posY, int width, int height, bool smooth)
{
	sf::Texture texture;

	if (!texture.loadFromFile(path, sf::IntRect(posX, posY, width, height)))
	{
		std::cerr << "Failed to load texture: " << textureName << " " <<
		          path << std::endl;
		return false;
	}

	texture.setSmooth(smooth);
	m_textureMap[textureName] = texture;

	return true;
}

bool addFont(const std::string& fontName, const std::string& path)
{
	// TODO
}

bool Assets::addAnimation(const std::string& animationName,
					const std::string& textureName, const size_t frameCount,
					const size_t duration)
{
	auto& texture = this->getTexture(textureName);
	m_animationMap[animationName] = Animation(animationName, texture); // TODO: frameCount & duration

	return true;
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const
{
	// TODO: handle not found case
	return m_textureMap.at(textureName);
}

const Animation& Assets::getAnimation(const std::string& animationName) const
{
	// TODO: handle not found case
	return m_animationMap.at(animationName);
}

const sf::Font& Assets::getFont(const std::string& fontName) const
{
	// TODO
}

void Assets::loadFromFile(const std::string& filePath)
{
	std::ifstream fin(filePath);
	std::string token;

	while (fin.good())
	{
		fin >> token;
		if (token == "Texture")
		{
			std::string name, path;
			int X, Y, width, height;
			bool smooth;
			fin >> name >> path >> X >> Y >> width >> height >> smooth;
			if (!this->addTexture(name, path, X, Y, width, height, smooth))
			{
				// TODO: handle error
			}
		}
		else if (token == "Animation")
		{
			std::string name, textureName;
			int frameCount, duration;
			fin >> name >> textureName >> frameCount >> duration;
			if (!this->addAnimation(name, textureName, frameCount, duration))
			{
				// TODO: handle error
			}
		}
	}
}