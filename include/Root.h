#ifndef ROOT_H
#define ROOT_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Resources.h"
#include "Game.h"

class Root
{
public:
    static Root& instance();

    void setActiveShader(const sf::Shader& shader);
    void unsetActiveShader();
    void setActiveTexture(const sf::Texture& texture);
    void unsetActiveTexture();
    void start();

    const sf::RenderWindow& window() const;
    sf::RenderWindow& window();
    const sf::RenderStates& renderStates() const;
    sf::RenderStates& renderStates();
    const Resources& resources() const;
    Resources& resources();

    Game& game();
    const Game& game() const;

private:
    Root();

    sf::RenderWindow m_window;
    sf::RenderStates m_renderStates;
    Resources m_resources;
    Game m_game;
};

#endif // ROOT_H
