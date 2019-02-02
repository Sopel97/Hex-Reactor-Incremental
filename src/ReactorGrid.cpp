#include "ReactorGrid.h"
#include "Game.h"
#include "Resources.h"
#include "Root.h"
#include "ComponentConditionBar.h"
#include "Component.h"
#include "SFMLUtil.h"
#include "BinaryStream.h"

#include <cmath>


ReactorGrid::GridRegionAroundComponent::GridRegionAroundComponent(const ReactorGrid& reactorGrid, const Vec2I& pos) :
    m_center(pos),
    m_component(reactorGrid.componentAt(pos)),
    m_neighbours(reactorGrid.adjacentComponents(pos))
{
}
ReactorGrid::GridRegionAroundComponent::GridRegionAroundComponent(ReactorGrid& reactorGrid, const Vec2I& pos) :
    m_center(pos),
    m_component(reactorGrid.componentAt(pos)),
    m_neighbours(reactorGrid.adjacentComponents(pos))
{
}

std::shared_ptr<Component>& ReactorGrid::GridRegionAroundComponent::component()
{
    return m_component;
}
const std::shared_ptr<Component>& ReactorGrid::GridRegionAroundComponent::component() const
{
    return m_component;
}
const std::vector<std::shared_ptr<Component>>& ReactorGrid::GridRegionAroundComponent::neighbours() const
{
    return m_neighbours;
}
std::vector<std::shared_ptr<Component>>& ReactorGrid::GridRegionAroundComponent::neighbours()
{
    return m_neighbours;
}
const Vec2I& ReactorGrid::GridRegionAroundComponent::center() const
{
    return m_center;
}

ReactorGrid::ReactorGrid(Game& game, Reactor& reactor, int width, int height) :
    m_game(game),
    m_reactor(reactor),
    m_components(width + height / 2 + 1, height, nullptr)
{
    m_viewRect = game.windowSpaceManager().windowRegionRect(WindowSpaceManager::WindowRegionId::ReactorGrid);
    m_gridOffset = Vec2F {16.0f, 16.0f};
    m_gridSizeInTiles = Vec2I {width, height};
    m_tileSize = Vec2F {48.0f, 48.0f};
    m_tileSizeAfterTiling = Vec2F {m_tileSize.x, m_tileSize.y* (3.0f / 4.0f)};
    m_gridSize = m_tileSizeAfterTiling * m_gridSizeInTiles + m_gridOffset * 2.0f;
    m_gridSize.y += m_tileSize.y * (1.0f / 4.0f);
    m_xOffset = (height - 1) / 2;
    m_camera = Vec2F {0.0f, 0.0f};

    /*
    for(auto& cell : m_components)
    {
        cell = game.componentFactory().createComponent("Capacitor - Tier 1");
    }
    */
}

void ReactorGrid::update(double dt)
{
    for(auto& anim : m_explosionAnimations)
    {
        anim.update(dt);
    }

    m_explosionAnimations.erase(std::remove_if(m_explosionAnimations.begin(), m_explosionAnimations.end(), [](const Animation & anim) {return anim.hasEnded();}), m_explosionAnimations.end());
}
const Vec2F& ReactorGrid::gridSize() const
{
    return m_gridSize;
}
const Vec2I& ReactorGrid::gridSizeInTiles() const
{
    return m_gridSizeInTiles;
}
const Vec2F& ReactorGrid::gridOffset() const
{
    return m_gridOffset;
}
const Vec2F& ReactorGrid::tileSize() const
{
    return m_tileSize;
}
const Vec2F& ReactorGrid::tileSizeAfterTiling() const
{
    return m_tileSizeAfterTiling;
}
const Array2<std::shared_ptr<Component>>& ReactorGrid::components() const
{
    return m_components;
}
Array2<std::shared_ptr<Component>>& ReactorGrid::components()
{
    return m_components;
}
std::shared_ptr<Component>& ReactorGrid::componentAt(const Vec2I& pos)
{
    return m_components(pos.x + m_xOffset, pos.y);
}
const std::shared_ptr<Component>& ReactorGrid::componentAt(const Vec2I& pos) const
{
    return m_components(pos.x + m_xOffset, pos.y);
}
bool ReactorGrid::isValid(const Vec2I& pos) const
{
    if(pos.y < 0) return false;
    if(pos.y > m_gridSizeInTiles.y - 1) return false;
    if(pos.x < -pos.y / 2) return false;
    if(pos.x > m_gridSizeInTiles.x - 1 - (pos.y + 1) / 2) return false;
    return true;
}
bool ReactorGrid::tryPlaceComponent(const Vec2I& pos, std::shared_ptr<Component>&& component)
{
    std::shared_ptr<Component>& toReplace = componentAt(pos);
    if(component->canReplace(toReplace))
    {
        placeComponent(pos, std::move(component));
        return true;
    }
    return false;
}
void ReactorGrid::placeComponent(const Vec2I& pos, std::shared_ptr<Component>&& component)
{
    componentAt(pos) = std::move(component);
}
void ReactorGrid::removeComponent(const Vec2I& pos)
{
    componentAt(pos) = nullptr;
}
void ReactorGrid::meltComponent(const Vec2I& pos)
{
    auto& component = componentAt(pos);
    if(component != nullptr)
    {
        createExplosionOnHex(pos);
        component = nullptr;
    }
}
void ReactorGrid::meltAllComponents()
{
    for(const auto& pos : possibleHexCoords())
    {
        meltComponent(pos);
    }
}
Vec2I ReactorGrid::coordsToHex(Vec2F coords) const
{
    coords -= m_gridOffset;
    coords -= m_tileSize / 2.0f;
    constexpr float sqrt3 = 1.7320508075688772935274463415059f;
    float edge = m_tileSize.x / sqrt3;
    float x = (coords.x * (1.0f / sqrt3) - coords.y * (2.0f / (3.0f * sqrt3))) / edge;
    float y = coords.y * (4.0f / (3.0f * sqrt3)) / edge;
    return roundToHex(Vec2F {x, y});
}

Vec2F ReactorGrid::hexToCoords(const Vec2I& hex) const
{
    constexpr float sqrt3 = 1.7320508075688772935274463415059f;
    float edge = m_tileSize.x / sqrt3;
    float x = (hex.x * sqrt3 + hex.y * (sqrt3 / 2.0f)) * edge;
    float y = hex.y * (9.0f / (4.0f * sqrt3)) * edge;
    return Vec2F {x, y} + m_gridOffset + m_tileSize / 2.0f;
}
Vec2I ReactorGrid::horizontalToAxial(const Vec2I& horizontal) const
{
    int x = horizontal.x - (horizontal.y - (horizontal.y & 1)) / 2;
    int y = horizontal.y;
    return Vec2I {x, y};
}

Vec2I ReactorGrid::roundToHex(const Vec2F& fractionalHexCoords) const
{
    return cubeToHex(roundToCube(hexToCube(fractionalHexCoords)));
}

Vec2F ReactorGrid::cubeToHex(const Vec3F& cube) const
{
    return Vec2F {cube.x, cube.z};
}
Vec2I ReactorGrid::cubeToHex(const Vec3I& cube) const
{
    return Vec2I {cube.x, cube.z};
}
Vec3F ReactorGrid::roundToCube(const Vec3F& fractionalCubeCoords) const
{
    float rx = std::round(fractionalCubeCoords.x);
    float ry = std::round(fractionalCubeCoords.y);
    float rz = std::round(fractionalCubeCoords.z);

    float xdiff = std::abs(rx - fractionalCubeCoords.x);
    float ydiff = std::abs(ry - fractionalCubeCoords.y);
    float zdiff = std::abs(rz - fractionalCubeCoords.z);

    if(xdiff > ydiff && xdiff > zdiff)
        rx = -ry - rz;
    else if(ydiff > zdiff)
        ry = -rx - rz;
    else
        rz = -rx - ry;

    return Vec3F {rx, ry, rz};
}
Vec3F ReactorGrid::hexToCube(const Vec2F& hex) const
{
    float x = hex.x;
    float z = hex.y;
    float y = -x - z;
    return Vec3F {x, y, z};
}
Vec3I ReactorGrid::hexToCube(const Vec2I& hex) const
{
    int x = hex.x;
    int z = hex.y;
    int y = -x - z;
    return Vec3I {x, y, z};
}
void ReactorGrid::draw()
{
    sf::RenderWindow& window = Root::instance().window();
    m_game.windowSpaceManager().setViewToWindowRegion(WindowSpaceManager::WindowRegionId::ReactorGrid, m_camera);
    Root::instance().setActiveShader(Root::instance().resources().shaders.worldSpaceNoiseMask); //will also be applied to walls later (or maybe to whole screen, we'll see)
    window.draw(sf::Sprite(Root::instance().resources().textures.grid), Root::instance().renderStates());
    Root::instance().unsetActiveShader();

    drawComponents();
    drawExplosions();

    Vec2I mousePos {sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y};
    auto pointedWindowRegion = m_game.windowSpaceManager().pointedWindowRegion(mousePos);
    if(pointedWindowRegion == nullptr) return;
    if(pointedWindowRegion->regionId() == WindowSpaceManager::WindowRegionId::ReactorGrid)
    {
        Vec2F mousePosOnGrid = pointedWindowRegion->localCoords(mousePos);
        Vec2I hex = coordsToHex(Vec2F {mousePosOnGrid.x + m_camera.x, mousePosOnGrid.y + m_camera.y});
        if(isValid(hex))
        {
            Vec2F hexCoords = hexToCoords(hex);
            sf::Sprite mask(Root::instance().resources().textures.hexTileMask);
            mask.setColor(sf::Color(255, 255, 255, 96));
            mask.setPosition(sf::Vector2f(hexCoords.x - m_tileSize.x / 2.0f, hexCoords.y - m_tileSize.y / 2.0f));
            window.draw(mask, Root::instance().renderStates());
        }
    }
    m_game.windowSpaceManager().setDefaultView();
}
void ReactorGrid::createExplosionOnHex(const Vec2I& hex)
{
    Vec2F hexCoords = hexToCoords(hex);
    Vec2F frameSize {32.0f, 32.0f};
    int numberOfFrames = 29;
    float frameTime = 0.04f;
    m_explosionAnimations.push_back(
        Animation(
            Root::instance().resources().textures.explosion,
            numberOfFrames,
            frameTime,
            hexCoords - frameSize / 2.0f,
            Vec2F {0.0f, 0.0f},
            Vec2F {frameSize.x, 0.0f},
            frameSize
        )
    );
}
void ReactorGrid::drawExplosions()
{
    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);
    for(auto& animation : m_explosionAnimations)
    {
        if(animation.hasEnded()) continue;
        animation.appendToVertexArray(vertexArray);
    }

    Root::instance().setActiveTexture(Root::instance().resources().textures.explosion);
    Root::instance().window().draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
}
std::vector<Vec2I> ReactorGrid::possibleHexCoords() const
{
    std::vector<Vec2I> hexes;
    int gridWidth = m_gridSizeInTiles.x;
    int gridHeight = m_gridSizeInTiles.y;
    hexes.reserve(gridWidth * gridHeight);
    for(int x = 0; x < gridWidth; ++x)
    {
        for(int y = 0; y < gridHeight; ++y)
        {
            Vec2I axialHexCoords = horizontalToAxial(Vec2I {x, y});
            if(!isValid(axialHexCoords)) continue;
            hexes.push_back(axialHexCoords);
        }
    }
    return hexes;
}

void ReactorGrid::emitHeat(const BigInt& amount, GridRegionAroundComponent& region)
{
    std::vector<std::shared_ptr<Component>> destinationComponents;
    BigInt remainingAmountToEmit = amount;
    for(auto& component : region.neighbours())
    {
        if(component->acceptsHeat()) destinationComponents.push_back(component);
    }
    int numberOfDestinationsLeft = destinationComponents.size();
    for(auto& component : destinationComponents)
    {
        BigInt amountToTransfer = remainingAmountToEmit / numberOfDestinationsLeft;

        component->addHeat(amountToTransfer);

        remainingAmountToEmit -= amountToTransfer;
        --numberOfDestinationsLeft;
    }
    m_reactor.addHeat(remainingAmountToEmit);
}
void ReactorGrid::drawComponents()
{
    auto isVisible = [](const RectangleI & rect, const Vec2F & pos) -> bool
    {
        return rect.intersects(RectangleI(pos, 32, 32));
    };
    const RectangleI& rect = m_game.windowSpaceManager().windowRegionRect(WindowSpaceManager::WindowRegionId::ReactorGrid);
    sf::RenderWindow& window = Root::instance().window();
    Vec2F offset = m_gridOffset;
    Vec2F tileSize = m_tileSize;
    int gridWidth = m_gridSizeInTiles.x;
    int gridHeight = m_gridSizeInTiles.y;
    sf::VertexArray componentVertexArray(sf::PrimitiveType::Triangles);
    sf::VertexArray barVertexArray(sf::PrimitiveType::Triangles);
    for(int x = 0; x < gridWidth; ++x)
    {
        for(int y = 0; y < gridHeight; ++y)
        {
            Vec2I axialHexCoords = horizontalToAxial(Vec2I {x, y});
            if(!isValid(axialHexCoords)) continue; //because odd rows have one less hex and axialHexCoords may point to the one hex that's not there
            std::shared_ptr<Component>& component = componentAt(axialHexCoords);
            if(component == nullptr) continue;
            Vec2F hexCenter = hexToCoords(axialHexCoords);

            if(!isVisible(rect, hexCenter - Vec2F {16.0f, 16.0f} + rect.min - m_camera)) continue;

            Vec2F spritePos = component->isDepleted() ? component->depletedSpritePosition() : component->spritePosition();

            Vec2F size {32.0f, 32.0f};
            Vec2F position = hexCenter - size / 2.0f;

            SFMLUtil::appendQuadAsTriangleList(componentVertexArray, position, size, spritePos, size);

            if(component->hasConditionBar())
            {
                component->conditionBar().draw(barVertexArray, hexCenter);
            }
        }
    }
    Root::instance().setActiveTexture(Root::instance().resources().textures.spritesheet);
    window.draw(componentVertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
    window.draw(barVertexArray, Root::instance().renderStates());
}
std::vector<std::shared_ptr<Component>> ReactorGrid::adjacentComponents(const Vec2I& hex) const
{
    std::vector<std::shared_ptr<Component>> components;
    components.reserve(6);
    static const Vec2I adjacentHexes[]
    {
        Vec2I{0, -1},
        Vec2I{1, -1},
        Vec2I{1, 0},
        Vec2I{0, 1},
        Vec2I{ -1, 1},
        Vec2I{ -1, 0}
    };
    for(const auto& offset : adjacentHexes)
    {
        Vec2I adjacentHex = hex + offset;
        if(!isValid(adjacentHex)) continue;
        auto& component = componentAt(adjacentHex);
        if(component == nullptr) continue;
        components.push_back(component);
    }
    return components;
}
void ReactorGrid::updateComponentsAfterUpgradesChanged()
{
    for(const auto& hex : possibleHexCoords())
    {
        auto& component = componentAt(hex);
        if(component != nullptr) component->updateAfterUpgradesChanged();
    }
}
const RectangleI& ReactorGrid::viewRect() const
{
    return m_viewRect;
}
void ReactorGrid::onWindowResized(sf::Event& windowResizedEvent)
{
    m_viewRect = m_game.windowSpaceManager().windowRegionRect(WindowSpaceManager::WindowRegionId::ReactorGrid);
    adjustCamera();
}
const Vec2F& ReactorGrid::camera() const
{
    return m_camera;
}
void ReactorGrid::updateCamera()
{
    moveCamera(Vec2F {0.0f, 0.0f});
}
void ReactorGrid::moveCamera(const Vec2F& displacement)
{
    m_camera += displacement;
    adjustCamera();
}
void ReactorGrid::adjustCamera()
{
    Vec2F gridViewSize = m_viewRect.max - m_viewRect.min;
    if(m_camera.x < 0) m_camera.x = 0;
    if(m_camera.y < 0) m_camera.y = 0;
    if(m_camera.x > m_gridSize.x - gridViewSize.x) m_camera.x = m_gridSize.x - gridViewSize.x;
    if(m_camera.y > m_gridSize.y - gridViewSize.y) m_camera.y = m_gridSize.y - gridViewSize.y;

    if(gridViewSize.x > m_gridSize.x) m_camera.x = (m_gridSize.x - gridViewSize.x) / 2.0f;
    if(gridViewSize.y > m_gridSize.y) m_camera.y = (m_gridSize.y - gridViewSize.y) / 2.0f;
}

int ReactorGrid::hexDistance(const Vec2I& first, const Vec2I& second) const
{
    Vec3I a = hexToCube(first);
    Vec3I b = hexToCube(second);
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z)) / 2;
}
void ReactorGrid::serialize(BinaryStream& stream) const
{
    int width = m_components.sizeX();
    int height = m_components.sizeY();
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            auto& component = m_components(x, y);
            if(component == nullptr)
            {
                stream << std::string("null");
            }
            else
            {
                stream << component->name();
                component->serialize(stream);
            }
        }
    }
}
void ReactorGrid::deserialize(BinaryStream& stream)
{
    int width = m_components.sizeX();
    int height = m_components.sizeY();
    for(int x = 0; x < width; ++x)
    {
        for(int y = 0; y < height; ++y)
        {
            std::string componentName;

            stream >> componentName;

            if(componentName == "null")
            {
                m_components(x,y) = nullptr;
            }
            else
            {
                auto component = m_game.componentFactory().createComponent(componentName);
                component->deserialize(stream);
                m_components(x,y) = std::move(component);
            }
        }
    }
}
