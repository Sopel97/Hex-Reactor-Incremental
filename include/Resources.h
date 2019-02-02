#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Resources
{
public:
    Resources();
    struct Textures
    {
        sf::Texture grid;
        sf::Texture hexTileMask;
        sf::Texture spritesheet;
        sf::Texture noise;
        sf::Texture font;
        sf::Texture uiBackground;
        sf::Texture barSegments;
        sf::Texture moneyBackground;
        sf::Texture explosion;
    } textures;

    struct Shaders
    {
        sf::Shader screenSpaceNoiseMask;
        sf::Shader worldSpaceNoiseMask;
    } shaders;

    struct Fonts
    {
        sf::Font descriptionFont;
        sf::Font standardFont;
    } fonts;
private:
};

#endif // RESOURCES_H
