#ifndef REACTORGRID_H
#define REACTORGRID_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../LibS/Array2.h"
#include "../LibS/Geometry.h"
#include <memory>

#include "Bignum.h"
#include "Animation.h"
#include "Serializable.h"

class Game;
class Component;
class Reactor;

using namespace Geo;

class ReactorGrid : public Serializable //hex grid is in axial coordinates
{
public:
    class GridRegionAroundComponent
    {
    public:
        GridRegionAroundComponent(const ReactorGrid& reactorGrid, const Vec2I& pos);
        GridRegionAroundComponent(ReactorGrid& reactorGrid, const Vec2I& pos);

        std::shared_ptr<Component>& component();
        const std::shared_ptr<Component>& component() const;
        std::vector<std::shared_ptr<Component>>& neighbours();
        const std::vector<std::shared_ptr<Component>>& neighbours() const;
        const Vec2I& center() const;
    protected:
        Vec2I m_center;
        std::shared_ptr<Component> m_component;
        std::vector<std::shared_ptr<Component>> m_neighbours;
    };

    ReactorGrid(Game& game, Reactor& reactor, int width, int height);
    std::shared_ptr<Component>& componentAt(const Vec2I& pos);
    const std::shared_ptr<Component>& componentAt(const Vec2I& pos) const;
    bool isValid(const Vec2I& pos) const;
    Vec2I coordsToHex(Vec2F coords) const;
    Vec2F hexToCoords(const Vec2I& hex) const;
    Vec2I roundToHex(const Vec2F& fractionHexCoords) const;
    Vec2I horizontalToAxial(const Vec2I& horizontal) const;
    std::vector<Vec2I> possibleHexCoords() const;

    void emitHeat(const BigInt& amount, GridRegionAroundComponent& region);

    const Vec2F& gridSize() const;
    const Vec2I& gridSizeInTiles() const;
    const Vec2F& gridOffset() const;
    const Vec2F& tileSize() const;
    const Vec2F& tileSizeAfterTiling() const;

    const Array2<std::shared_ptr<Component>>& components() const;
    Array2<std::shared_ptr<Component>>& components();

    std::vector<std::shared_ptr<Component>> adjacentComponents(const Vec2I& hex) const;

    void updateComponentsAfterUpgradesChanged();

    void update(double dt);
    void draw();
    void drawExplosions();
    void drawComponents();
    const Vec2F& camera() const;
    void updateCamera();
    void moveCamera(const Vec2F& displacement);
    void adjustCamera();

    const RectangleI& viewRect() const;

    bool tryPlaceComponent(const Vec2I& pos, std::shared_ptr<Component>&& component);
    void placeComponent(const Vec2I& pos, std::shared_ptr<Component>&& component);
    void removeComponent(const Vec2I& pos);
    void meltComponent(const Vec2I& pos);
    void meltAllComponents();

    void createExplosionOnHex(const Vec2I& hex);

    void onWindowResized(sf::Event& windowResizedEvent);

    int hexDistance(const Vec2I& first, const Vec2I& second) const;

    virtual void serialize(BinaryStream& stream) const;
    virtual void deserialize(BinaryStream& stream);

private:
    Game& m_game;
    Reactor& m_reactor;
    //nullptr == empty tile
    Array2<std::shared_ptr<Component>> m_components;
    std::vector<Animation> m_explosionAnimations;
    RectangleI m_viewRect;
    Vec2F m_gridSize;
    Vec2I m_gridSizeInTiles;
    Vec2F m_gridOffset;
    Vec2F m_tileSize;
    Vec2F m_tileSizeAfterTiling; //not sprite size but size after tiling. So it means how many pixels there is from top left corner of one tile to the top left corner of the tile to the bottom right
    int m_xOffset;
    Vec2F m_camera;

    Vec2F cubeToHex(const Vec3F& cube) const;
    Vec3F hexToCube(const Vec2F& hex) const;
    Vec2I cubeToHex(const Vec3I& cube) const;
    Vec3I hexToCube(const Vec2I& hex) const;
    Vec3F roundToCube(const Vec3F& fractionslCubeCoords) const;

};

#endif // REACTORGRID_H
