#include "Root.h"

Root::Root() :
    m_window(sf::VideoMode(1024, 768), "Hex Reactor Incremental"),
    m_renderStates(),
    m_resources(),
    m_game(m_window)
{
}
Root& Root::instance()
{
    static Root root;
    return root;
}

void Root::start()
{
    m_game.start();
}
sf::RenderWindow& Root::window()
{
    return m_window;
}
const sf::RenderWindow& Root::window() const
{
    return m_window;
}
sf::RenderStates& Root::renderStates()
{
    return m_renderStates;
}
const sf::RenderStates& Root::renderStates() const
{
    return m_renderStates;
}
void Root::setActiveShader(const sf::Shader& shader)
{
    m_renderStates.shader = &shader;
}
void Root::unsetActiveShader()
{
    m_renderStates.shader = nullptr;
}
void Root::setActiveTexture(const sf::Texture& texture)
{
    m_renderStates.texture = &texture;
}
void Root::unsetActiveTexture()
{
    m_renderStates.texture = nullptr;
}
const Resources& Root::resources() const
{
    return m_resources;
}
Resources& Root::resources()
{
    return m_resources;
}
Game& Root::game()
{
    return m_game;
}
const Game& Root::game() const
{
    return m_game;
}
