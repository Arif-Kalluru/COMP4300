#pragma once

#include "Animation.hpp"

#include <map>

class Assets
{
private:
	std::map<std::string, sf::Texture> m_textureMap;
	std::map<std::string, Animation>   m_animationMap;
	std::map<std::string, sf::Font>    m_fontMap;

	bool addTexture(const std::string& textureName, const std::string& path,
	                bool smooth = true);
	bool addTexture(const std::string& textureName, const std::string& path,
	                int posX, int posY, int width, int height,
					bool smooth = true);                                        // (posX, posY) is (rectLeft, rectTop)
	bool addFont(const std::string& fontName, const std::string& path);
	bool addAnimation(const std::string& animationName,
	                  const std::string& textureName, const size_t frameCount,
					  const size_t duration);

public:
	Assets();

	const sf::Texture& getTexture(const std::string& textureName)     const;
	const Animation&   getAnimation(const std::string& animationName) const;
	const sf::Font&    getFont(const std::string& fontName)           const;

	void loadFromFile(const std::string& filePath);
};