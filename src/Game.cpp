#include "Game.h"
#include "Cell.h"
#include "TextRenderer.h"
#include "Configuration.h"
#include "Root.h"
#include "BinaryStream.h"

#include <fstream>

Game::Game(sf::RenderWindow& window) :
    m_window(window),
    m_windowSpaceManager(window),
    m_componentFactory("assets\\cfg\\components.cfg"),
    m_userInterface(*this, window),
    m_componentShop(*this, window),
    m_upgradeShop(*this, window, WindowSpaceManager::WindowRegionId::UpgradesScreenPanelInner),
    m_reactor(*this, 33, 33),
    m_tickTime(1.0 / 60.0 * 1000.0),
    m_reactorTickTime(1000.0),
    m_isReactorEnabled(true),
    m_framesSinceLastFPSMeasure(0),
    m_lastFPSMeasureTime(0),
    m_lastFPSMeasured(0),
    m_lastFrameTime(0.0f)
{
    m_lastMousePos = Vec2I {sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y};

    m_componentShop.addDisplay("assets\\cfg\\leftComponentShopDisplay.cfg", WindowSpaceManager::WindowRegionId::LeftPanelInner);
    m_componentShop.addDisplay("assets\\cfg\\rightComponentShopDisplay.cfg", WindowSpaceManager::WindowRegionId::RightPanelInner);
}

void Game::draw()
{
    m_window.clear();

    if(m_windowSpaceManager.currentScene() == WindowSpaceManager::SceneId::Main)
    {
        m_reactor.draw();
        m_userInterface.draw();
        m_componentShop.draw();
    }
    else //upgrade screen
    {
        m_userInterface.draw();
        m_upgradeShop.draw();
    }

    m_window.display();
    ++m_framesSinceLastFPSMeasure;
}

void Game::tick()
{
    double dt = m_tickTime / 1000.0;
    processAsyncKeyboardInput();
    processAsyncMouseInput();
    if(m_windowSpaceManager.currentScene() == WindowSpaceManager::SceneId::Main)
    {
        m_componentShop.update(dt);
    }
    else if(m_windowSpaceManager.currentScene() == WindowSpaceManager::SceneId::Upgrades)
    {
        m_upgradeShop.update(dt);
    }
    m_reactor.update(dt);

    m_lastMousePos = Vec2I {sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y};
}
void Game::reactorTick()
{
    if(m_isReactorEnabled) m_reactor.tick();
}
bool Game::isReactorEnabled() const
{
    return m_isReactorEnabled;
}
void Game::enableReactor()
{
    m_isReactorEnabled = true;
}
void Game::disableReactor()
{
    m_isReactorEnabled = false;
}

bool Game::isAutoCellReplacingEnabled() const
{
    return m_isAutoCellReplacingEnabled;
}
void Game::enableAutoCellReplacing()
{
    m_isAutoCellReplacingEnabled = true;
}
void Game::disableAutoCellReplacing()
{
    m_isAutoCellReplacingEnabled = false;
}

bool Game::isAutoNeutronReflectorReplacingEnabled() const
{
    return m_isAutoNeutronReflectorReplacingEnabled;
}
void Game::enableAutoNeutronReflectorReplacing()
{
    m_isAutoNeutronReflectorReplacingEnabled = true;
}
void Game::disableAutoNeutronReflectorReplacing()
{
    m_isAutoNeutronReflectorReplacingEnabled = false;
}
void Game::start()
{
    int lastTick;
    int lastReactorTick;
    int lastDraw;
    m_clock.restart();
    lastTick = m_clock.getElapsedTime().asMilliseconds();
    lastReactorTick = lastTick;
    lastDraw = lastTick;
    m_lastFPSMeasureTime = lastTick;
    while(m_window.isOpen())
    {
        sf::Event event;

        while(m_window.pollEvent(event))
        {
            if(event.type == sf::Event::EventType::Closed) m_window.close();
            if(event.type == sf::Event::EventType::Resized) onWindowResized(event);
            if(m_window.hasFocus()) if(event.type == sf::Event::EventType::KeyPressed || event.type == sf::Event::EventType::KeyReleased) processKeyboardInput(event);
            if(m_window.hasFocus()) if(event.type == sf::Event::EventType::MouseButtonPressed ||
                                           event.type == sf::Event::EventType::MouseButtonReleased ||
                                           event.type == sf::Event::EventType::MouseWheelMoved) processMouseInput(event);
        }
        sf::Time elapsedTime = m_clock.getElapsedTime();
        if(elapsedTime.asMilliseconds() >= m_tickTime + lastTick)
        {
            tick();
            lastTick = elapsedTime.asMilliseconds();
        }
        if(elapsedTime.asMilliseconds() >= m_reactorTickTime + lastReactorTick)
        {
            reactorTick();
            lastReactorTick = elapsedTime.asMilliseconds();
        }
        if(m_window.hasFocus())
        {
            draw(); //temporarly outside just to see how fps is doing
            int dt = elapsedTime.asMilliseconds() - lastDraw;
            m_lastFrameTime = dt / 1000.0f;
            lastDraw = elapsedTime.asMilliseconds();
        }
        if(m_lastFPSMeasureTime < elapsedTime.asMilliseconds() - 1000)
        {
            m_lastFPSMeasureTime = elapsedTime.asMilliseconds();
            m_lastFPSMeasured = m_framesSinceLastFPSMeasure;
            m_framesSinceLastFPSMeasure = 0;
        }

    }
}

void Game::processKeyboardInput(sf::Event& keyEvent)
{
    if(keyEvent.type == sf::Event::EventType::KeyPressed)
    {
        if(keyEvent.key.code == sf::Keyboard::Key::Space) //temporary
        {
            m_reactor.sellPower();
        }
        else if(keyEvent.key.code == sf::Keyboard::Key::V) //temporary, changes scene
        {
            m_windowSpaceManager.setCurrentScene(WindowSpaceManager::SceneId::Main);
        }
        else if(keyEvent.key.code == sf::Keyboard::Key::B) //temporary, changes scene
        {
            m_windowSpaceManager.setCurrentScene(WindowSpaceManager::SceneId::Upgrades);
        }
        else if(keyEvent.key.code == sf::Keyboard::Key::K) //temporary, saves game to file
        {
            saveGame("save001.save");
        }
        else if(keyEvent.key.code == sf::Keyboard::Key::L) //temporary, saves game to file
        {
            loadGame("save001.save");
        }
    }
    else if(keyEvent.type == sf::Event::EventType::KeyReleased)
    {

    }
}
void Game::processAsyncKeyboardInput()
{
    constexpr float gridCameraSpeed = 9.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) m_reactor.reactorGrid().moveCamera(Vec2F {0.0f, -gridCameraSpeed});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) m_reactor.reactorGrid().moveCamera(Vec2F {0.0f, gridCameraSpeed});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) m_reactor.reactorGrid().moveCamera(Vec2F { -gridCameraSpeed, 0.0f});
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) m_reactor.reactorGrid().moveCamera(Vec2F {gridCameraSpeed, 0.0f});

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) m_reactor.addHeat(50);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) m_reactor.addHeat(-50);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) m_reactor.addPower(50);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) m_reactor.addPower(-50);
}

void Game::processMouseInput(sf::Event& mouseEvent)
{
    if(mouseEvent.type == sf::Event::EventType::MouseButtonPressed)
    {
        Vec2I mousePos {mouseEvent.mouseButton.x, mouseEvent.mouseButton.y};
        m_userInterface.onMouseButtonPressed(mousePos);
        auto pointedWindowRegion = m_windowSpaceManager.pointedWindowRegion(mousePos);
        if(pointedWindowRegion == nullptr) return;
        else if(pointedWindowRegion->regionId() == WindowSpaceManager::WindowRegionId::UpgradesScreenPanelInner)
        {
            m_upgradeShop.onMouseButtonPressed(pointedWindowRegion->localCoords(mousePos));
        }
    }
    else if(mouseEvent.type == sf::Event::EventType::MouseButtonReleased)
    {

    }
    else if(mouseEvent.type == sf::Event::EventType::MouseWheelMoved)
    {
        Vec2I mousePos {mouseEvent.mouseWheel.x, mouseEvent.mouseWheel.y};
        auto pointedWindowRegion = m_windowSpaceManager.pointedWindowRegion(mousePos);
        if(pointedWindowRegion == nullptr) return;
        else if(pointedWindowRegion->regionId() == WindowSpaceManager::WindowRegionId::UpgradesScreenPanelInner)
        {
            m_upgradeShop.moveCamera(Vec2F(0.0f, -mouseEvent.mouseWheel.delta * 45.0f));
        }
    }

    m_componentShop.processMouseInput(mouseEvent);
}
void Game::processAsyncMouseInput()
{
    Vec2I mousePos {sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y};
    auto pointedWindowRegion = m_windowSpaceManager.pointedWindowRegion(mousePos);
    if(pointedWindowRegion == nullptr) return;
    if(pointedWindowRegion->regionId() == WindowSpaceManager::WindowRegionId::ReactorGrid)
    {
        Vec2F mousePosOnGrid = pointedWindowRegion->localCoords(mousePos);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_componentShop.selectedComponent() != nullptr) sweepPlaceComponents(pointedWindowRegion->localCoords(m_lastMousePos), mousePosOnGrid);
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) sweepRemoveComponents(pointedWindowRegion->localCoords(m_lastMousePos), mousePosOnGrid);

    }
}
void Game::sweepPlaceComponents(const Vec2I& first, const Vec2I& second) //position is relative to grid
{
    auto& reactorGrid = m_reactor.reactorGrid();
    Vec2F gridCamera = reactorGrid.camera();
    Vec2I firstHex = reactorGrid.coordsToHex(Vec2F {first.x + gridCamera.x, first.y + gridCamera.y});
    Vec2I lastHex = reactorGrid.coordsToHex(Vec2F {second.x + gridCamera.x, second.y + gridCamera.y});
    Vec2F firstHexCenter = reactorGrid.hexToCoords(firstHex);
    Vec2F lastHexCenter = reactorGrid.hexToCoords(lastHex);
    int hexDistance = reactorGrid.hexDistance(firstHex, lastHex);
    for(int i = 0; i <= hexDistance; ++i)
    {
        Vec2F sampledPoint;
        if(hexDistance == 0) sampledPoint = firstHexCenter;
        else sampledPoint = firstHexCenter + (lastHexCenter - firstHexCenter) * (1.0f / hexDistance) * i;
        Vec2I hex = reactorGrid.coordsToHex(sampledPoint);

        if(!buyAndPlaceSelectedComponent(hex)) break;
    }
}
void Game::sweepRemoveComponents(const Vec2I& first, const Vec2I& second) //position is relative to grid
{
    auto& reactorGrid = m_reactor.reactorGrid();
    Vec2F gridCamera = reactorGrid.camera();
    Vec2I firstHex = reactorGrid.coordsToHex(Vec2F {first.x + gridCamera.x, first.y + gridCamera.y});
    Vec2I lastHex = reactorGrid.coordsToHex(Vec2F {second.x + gridCamera.x, second.y + gridCamera.y});
    Vec2F firstHexCenter = reactorGrid.hexToCoords(firstHex);
    Vec2F lastHexCenter = reactorGrid.hexToCoords(lastHex);
    int hexDistance = reactorGrid.hexDistance(firstHex, lastHex);
    for(int i = 0; i <= hexDistance; ++i)
    {
        Vec2F sampledPoint;
        if(hexDistance == 0) sampledPoint = firstHexCenter;
        else sampledPoint = firstHexCenter + (lastHexCenter - firstHexCenter) * (1.0f / hexDistance) * i;
        Vec2I hex = reactorGrid.coordsToHex(sampledPoint);
        sellAndRemoveComponent(hex);
    }
}
int Game::lastFPSMeasured() const
{
    return m_lastFPSMeasured;
}
const ComponentFactory& Game::componentFactory() const
{
    return m_componentFactory;
}
ComponentFactory& Game::componentFactory()
{
    return m_componentFactory;
}
Reactor& Game::reactor()
{
    return m_reactor;
}

const Reactor& Game::reactor() const
{
    return m_reactor;
}

const PlayerStats& Game::playerStats() const
{
    return m_playerStats;
}
PlayerStats& Game::playerStats()
{
    return m_playerStats;
}
std::string Game::descriptionToShow() const //make this function have more logical flow
{
    Vec2I mousePos {sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y};
    auto pointedWindowRegion = m_windowSpaceManager.pointedWindowRegion(mousePos);
    std::shared_ptr<Component> pointedComponent = nullptr;
    if(pointedWindowRegion == nullptr)
    {
        auto& selectedComponent = m_componentShop.selectedComponent();
        if(selectedComponent != nullptr)
        {
            pointedComponent = m_componentFactory.component(selectedComponent->name());
        }
        if(pointedComponent != nullptr)
        {
            return pointedComponent->formattedOnShopDisplayDescription();
        }
        return "";
    }
    Vec2I localCoords = pointedWindowRegion->localCoords(mousePos);

    if(pointedWindowRegion->regionId() == WindowSpaceManager::WindowRegionId::ReactorGrid)
    {
        const ReactorGrid& reactorGrid = m_reactor.reactorGrid();
        Vec2I hex = reactorGrid.coordsToHex(localCoords);
        if(reactorGrid.isValid(hex))
        {
            pointedComponent = reactorGrid.componentAt(hex);
        }
        if(pointedComponent != nullptr)
        {
            return pointedComponent->formattedOnGridDescription(ReactorGrid::GridRegionAroundComponent(reactorGrid, hex));
        }
    }
    else
    {
        pointedComponent = m_componentShop.pointedComponent(mousePos); //must be window coords
    }
    if(pointedComponent == nullptr)
    {
        pointedComponent = m_componentShop.selectedComponent();
    }
    if(pointedComponent != nullptr)
    {
        return pointedComponent->formattedOnShopDisplayDescription();
    }

    return "";
}
bool Game::buyAndPlaceSelectedComponent(const Vec2I& hex)
{
    if(!m_reactor.reactorGrid().isValid(hex)) return false;
    std::shared_ptr<Component> component = m_componentFactory.createComponent(m_componentShop.selectedComponent()->name());
    if(component == nullptr) return false;
    BigInt componentPrice = component->price();
    component->setBaseValue(componentPrice);
    if(m_playerStats.money() < componentPrice) return false;
    if(m_reactor.reactorGrid().tryPlaceComponent(hex, std::move(component)))
    {
        m_playerStats.addMoney(-componentPrice);
        return true;
    }
    return false;
}
bool Game::sellAndRemoveComponent(const Vec2I& hex)
{
    if(!m_reactor.reactorGrid().isValid(hex)) return false;
    std::shared_ptr<Component>& component = m_reactor.reactorGrid().componentAt(hex);
    if(component == nullptr) return false;
    BigInt sellValue = component->sellValue();
    m_playerStats.addMoney(sellValue);
    m_reactor.reactorGrid().removeComponent(hex);
    return true;
}

WindowSpaceManager& Game::windowSpaceManager()
{
    return m_windowSpaceManager;
}
const WindowSpaceManager& Game::windowSpaceManager() const
{
    return m_windowSpaceManager;
}

void Game::onWindowResized(sf::Event& windowResizedEvent)
{
    if(m_window.getSize().x < 800 || m_window.getSize().y < 600) m_window.setSize(sf::Vector2u(std::max(800u, m_window.getSize().x), std::max(600u, m_window.getSize().y)));
    m_windowSpaceManager.onWindowResized(windowResizedEvent);
    m_reactor.onWindowResized(windowResizedEvent);
    m_componentShop.onWindowResized(windowResizedEvent);
    m_upgradeShop.onWindowResized(windowResizedEvent);
}

float Game::lastFrameTime() const
{
    return m_lastFrameTime;
}
void Game::saveGame(const std::string& fileName) const
{
    std::cout << "Saving game...\n";

    std::ofstream outputFile("saves\\" + fileName, std::ios::binary);
    BinaryStream binaryStream;

    //serializing objects...

    binaryStream << m_isReactorEnabled;
    binaryStream << m_isAutoCellReplacingEnabled;
    binaryStream << m_isAutoNeutronReflectorReplacingEnabled;

    m_playerStats.serialize(binaryStream);
    UpgradePool::instance().serialize(binaryStream);
    m_reactor.serialize(binaryStream);

    //end of serializing objects...

    const std::string& binaryStreamData = binaryStream.str();
    outputFile.write(binaryStreamData.data(), binaryStreamData.size());

    std::cout << "Game saved successfully...\n";
}

void Game::loadGame(const std::string& fileName)
{
    std::cout << "Loading game...\n";

    std::ifstream inputFile("saves\\" + fileName, std::ios::binary);

    std::string contents;
    inputFile.seekg(0, std::ios::end);
    contents.resize(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);
    inputFile.read(&contents[0], contents.size());

    BinaryStream binaryStream(std::move(contents));

    //deserializing objects...

    binaryStream >> m_isReactorEnabled;
    binaryStream >> m_isAutoCellReplacingEnabled;
    binaryStream >> m_isAutoNeutronReflectorReplacingEnabled;

    m_playerStats.deserialize(binaryStream);
    UpgradePool::instance().deserialize(binaryStream);
    m_reactor.deserialize(binaryStream);

    //end of deserializing objects...

    std::cout << "Game loaded successfully...\n";
}
