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

bool Assets::addFont(const std::string& fontName, const std::string& path)
{
    sf::Font font;

    if (!font.loadFromFile(path))
    {
        std::cerr << "Failed to load font: " << fontName << " " << path <<
            std::endl;
        return false;
    }

    m_fontMap[fontName] = font;

    return true;
}

bool Assets::addAnimation(const std::string& animationName,
                          const std::string& textureName,
                          const size_t keyframesCount, const size_t duration)
{
    const auto& texture = getTexture(textureName);
    m_animationMap[animationName] = Animation(
										animationName, texture, keyframesCount,
										duration
									);
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
    // TODO: handle not found case
    return m_fontMap.at(fontName);
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
            if (!addTexture(name, path, X, Y, width, height, smooth))
            {
                // TODO: handle error
            }
        }
        else if (token == "Animation")
        {
            std::string name, textureName;
            int keyframesCount, duration;
            fin >> name >> textureName >> keyframesCount >> duration;
            if (!addAnimation(name, textureName, keyframesCount, duration))
            {
                // TODO: handle error
            }
        }
        else if (token == "Font")
        {
            std::string name, path;
            fin >> name >> path;
            if (!addFont(name, path))
            {
                // TODO: handle error
            }
        }
    }
}