#include "UserInterface.h"
#include "WindowSpaceManager.h"
#include "Resources.h"
#include "TextRenderer.h"
#include "Game.h"
#include "Root.h"
#include "SFMLUtil.h"

#include <cmath>
#include <string>

#include "../LibS/Random.h"

using namespace Random;

UserInterface::UserInterface(Game& game, sf::RenderWindow& window) :
    m_game(game),
    m_window(window)
{
}
void UserInterface::draw()
{
    m_game.windowSpaceManager().setDefaultView();
    if(m_game.windowSpaceManager().currentScene() == WindowSpaceManager::SceneId::Main) drawMainScene();
    else if(m_game.windowSpaceManager().currentScene() == WindowSpaceManager::SceneId::Upgrades) drawUpgradesScene();
    drawFramerate();

    drawSceneControls();

}
void UserInterface::drawMainScene()
{
    drawWalls(m_game.windowSpaceManager().wallsOnScene(WindowSpaceManager::SceneId::Main));
    drawMainScenePanels();
    drawReactorBars();

    //TextRenderer::instance().drawFancyNumber("12345 6789.123 4567890 KM BTqQsS OND45678900/", m_window, Vec2F {300.f, 300.f}, TextRenderer::Alignment::Center);

    std::string description = m_game.descriptionToShow();
    TextRenderer::instance().drawDescription(description, m_window, Root::instance().renderStates(), m_game.windowSpaceManager().windowRegionRect(WindowSpaceManager::WindowRegionId::BottomPanel), TextRenderer::Alignment::Center);
}
void UserInterface::drawSceneControls()
{
    auto& sceneControls = m_game.windowSpaceManager().sceneControls();
    Vec2I sceneControlsOffset = m_game.windowSpaceManager().sceneControlsOffset();
    sceneControls.disableAllSegments();

    sceneControls.segment(
        m_game.windowSpaceManager().currentScene() == WindowSpaceManager::SceneId::Main
        ?
        "ReactorSelected"
        :
        "ReactorUnselected"
    ).enable();

    sceneControls.segment(
        m_game.windowSpaceManager().currentScene() == WindowSpaceManager::SceneId::Upgrades
        ?
        "UpgradesSelected"
        :
        "UpgradesUnselected"
    ).enable();

    sceneControls.segment(
        m_game.isReactorEnabled()
        ?
        "ReactorOn"
        :
        "ReactorOff"
    ).enable();

    sceneControls.segment(
        m_game.isAutoCellReplacingEnabled()
        ?
        "AutoReplaceCellsOn"
        :
        "AutoReplaceCellsOff"
    ).enable();

    sceneControls.segment(
        m_game.isAutoNeutronReflectorReplacingEnabled()
        ?
        "AutoReplaceNeutronReflectorsOn"
        :
        "AutoReplaceNeutronReflectorsOff"
    ).enable();

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);
    sceneControls.draw(vertexArray, sceneControlsOffset);
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;
    Root::instance().setActiveTexture(sprites);
    m_window.draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
}
void UserInterface::drawUpgradesScene()
{
    WindowSpaceManager& windowSpaceManager = m_game.windowSpaceManager();
    drawWalls(m_game.windowSpaceManager().wallsOnScene(WindowSpaceManager::SceneId::Upgrades));
    std::vector<RectangleI> panels;
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::UpgradesScreenPanel));
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::TopPanel));
    drawPanels(panels);
    drawReactorBars();
}
void UserInterface::drawFramerate()
{
    TextRenderer::instance().drawFancyNumber(std::to_string(m_game.lastFPSMeasured()), m_window, Root::instance().renderStates(), Vec2F {3.0f, 15.0f}, TextRenderer::Alignment::Left);
}
void UserInterface::drawPanel(const RectangleI& rect)
{
    drawPanels({rect});
}
void UserInterface::drawPanelBackground(const RectangleI& rect)
{
    drawPanelsBackgrounds({rect});
}
void UserInterface::drawPanelGradientBorder(const RectangleI& rect)
{
    drawPanelsGradientBorders({rect});
}
void UserInterface::drawPanelScrews(const RectangleI& rect)
{
    drawPanelsScrews({rect});
}
void UserInterface::drawPanelFrame(const RectangleI& rect)
{
    drawPanelsFrames({rect});
}
void UserInterface::drawPanels(const std::vector<RectangleI>& rects)
{
    drawPanelsBackgrounds(rects);
    drawPanelsGradientBorders(rects);
    drawPanelsScrews(rects);
    drawPanelsFrames(rects);
}
void UserInterface::drawPanelsBackgrounds(const std::vector<RectangleI>& rects)
{
    const sf::Texture& backgroundTexture = Root::instance().resources().textures.uiBackground;

    Root::instance().setActiveShader(Root::instance().resources().shaders.worldSpaceNoiseMask);

    sf::RectangleShape backgroundSprite;
    backgroundSprite.setFillColor(sf::Color::White);
    backgroundSprite.setTexture(&backgroundTexture);

    for(const auto& rect : rects)
    {
        backgroundSprite.setPosition(sf::Vector2f(rect.min.x, rect.min.y));
        backgroundSprite.setSize(sf::Vector2f(rect.width(), rect.height()));
        backgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(rect.width(), rect.height())));
        m_window.draw(backgroundSprite, Root::instance().renderStates());
    }

    Root::instance().unsetActiveShader();
}
void UserInterface::drawPanelsGradientBorders(const std::vector<RectangleI>& rects)
{
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;

    constexpr int gradientSize = 40;
    constexpr int numberOfGradientSprites = 8;
    constexpr int baseGradientSpriteX = 704;
    constexpr int baseGradientSpriteY = 14;

    sf::Vector2f spriteSize(gradientSize, gradientSize);

    sf::Vector2f gradientSpritePositions[numberOfGradientSprites] = {sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY),
                                                                     sf::Vector2f(baseGradientSpriteX + 2 * gradientSize, baseGradientSpriteY),
                                                                     sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY + 2 * gradientSize),
                                                                     sf::Vector2f(baseGradientSpriteX + 2 * gradientSize, baseGradientSpriteY + 2 * gradientSize),

                                                                     sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY + gradientSize),
                                                                     sf::Vector2f(baseGradientSpriteX + 2 * gradientSize, baseGradientSpriteY + gradientSize),
                                                                     sf::Vector2f(baseGradientSpriteX + gradientSize, baseGradientSpriteY),
                                                                     sf::Vector2f(baseGradientSpriteX + gradientSize, baseGradientSpriteY + 2 * gradientSize)
                                                                    };

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for(const auto& rect : rects)
    {
        sf::Vector2f gradientPositions[numberOfGradientSprites] = {sf::Vector2f(rect.min.x, rect.min.y),                                //top left
                                                                   sf::Vector2f(rect.max.x - gradientSize, rect.min.y),                 //top right
                                                                   sf::Vector2f(rect.min.x, rect.max.y - gradientSize),                 //bottom left
                                                                   sf::Vector2f(rect.max.x - gradientSize, rect.max.y - gradientSize),  //bottom right

                                                                   sf::Vector2f(rect.min.x, rect.min.y + gradientSize),                 //left
                                                                   sf::Vector2f(rect.max.x - gradientSize, rect.min.y + gradientSize),  //right
                                                                   sf::Vector2f(rect.min.x + gradientSize, rect.min.y),                 //top
                                                                   sf::Vector2f(rect.min.x + gradientSize, rect.max.y - gradientSize)   //down
                                                                  };
        sf::Vector2f gradientSizes[numberOfGradientSprites] = {sf::Vector2f(gradientSize, gradientSize),
                                                               sf::Vector2f(gradientSize, gradientSize),
                                                               sf::Vector2f(gradientSize, gradientSize),
                                                               sf::Vector2f(gradientSize, gradientSize),

                                                               sf::Vector2f(gradientSize, rect.height() - 2 * gradientSize),
                                                               sf::Vector2f(gradientSize, rect.height() - 2 * gradientSize),
                                                               sf::Vector2f(rect.width() - 2 * gradientSize, gradientSize),
                                                               sf::Vector2f(rect.width() - 2 * gradientSize, gradientSize),
                                                              };

        for(int i = 0; i < numberOfGradientSprites; ++i)
        {
            sf::Vector2f& position = gradientPositions[i];
            sf::Vector2f& size = gradientSizes[i];
            sf::Vector2f& spritePosition = gradientSpritePositions[i];

            SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
        }
    }
    Root::instance().setActiveTexture(sprites);
    m_window.draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
}
void UserInterface::drawPanelsScrews(const std::vector<RectangleI>& rects)
{
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;

    constexpr float averageSpaceBetweenScrews = 80.0f;
    constexpr float screwOffsetFromBorder = 20.0f;
    constexpr int numberOfScrewSprites = 2;
    constexpr int baseScrewSpriteX = 918;
    constexpr int baseScrewSpriteY = 14;
    constexpr int screwSizeX = 21;
    constexpr int screwSizeY = 25;
    sf::Vector2f screwSize(screwSizeX, screwSizeY);

    Xorshift32Engine randomEngine(123); //always seeded with the same number so it gives the same order of screw sprites for the same setup of them
    sf::Vector2f screwSpritePositions[numberOfScrewSprites] = {sf::Vector2f(baseScrewSpriteX, baseScrewSpriteY),
                                                               sf::Vector2f(baseScrewSpriteX + screwSizeX, baseScrewSpriteY)
                                                              };

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for(const auto& rect : rects)
    {
        int numberOfScrewsHorizontal = ((rect.width() - 2.0f * screwOffsetFromBorder) / averageSpaceBetweenScrews) + 2;
        int numberOfScrewsVertical = ((rect.height() - 2.0f * screwOffsetFromBorder) / averageSpaceBetweenScrews) + 2;
        float spaceBetweenScrewsHorizontal = (rect.width() - 2.0f * screwOffsetFromBorder) / (numberOfScrewsHorizontal - 1);
        float spaceBetweenScrewsVertical = (rect.height() - 2.0f * screwOffsetFromBorder) / (numberOfScrewsVertical - 1);

        for(int i = 1; i < numberOfScrewsHorizontal - 1; ++i) //corner screw will have been drawn in vetical batch
        {
            sf::Vector2f spritePosition = screwSpritePositions[randomEngine.nextInt(0, numberOfScrewSprites - 1)];
            sf::Vector2f position(screwOffsetFromBorder + i * spaceBetweenScrewsHorizontal + rect.min.x - screwSizeX / 2, screwOffsetFromBorder + rect.min.y - screwSizeY / 2);
            {
                sf::Vector2f& size = screwSize;
                sf::Vector2f& spriteSize = screwSize;

                SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
            }
            {
                position.y = rect.max.y - screwOffsetFromBorder - screwSizeY / 2;
                sf::Vector2f& size = screwSize;
                sf::Vector2f& spriteSize = screwSize;

                SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
            }
        }
        for(int i = 0; i < numberOfScrewsVertical; ++i)
        {
            sf::Vector2f spritePosition = screwSpritePositions[randomEngine.nextInt(0, numberOfScrewSprites - 1)];
            sf::Vector2f position(screwOffsetFromBorder + rect.min.x - screwSizeX / 2, screwOffsetFromBorder + i * spaceBetweenScrewsVertical + rect.min.y - screwSizeY / 2);
            {
                sf::Vector2f& size = screwSize;
                sf::Vector2f& spriteSize = screwSize;

                SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
            }
            {
                position.x = rect.max.x - screwOffsetFromBorder - screwSizeX / 2;
                sf::Vector2f& size = screwSize;
                sf::Vector2f& spriteSize = screwSize;

                SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
            }
        }
    }
    Root::instance().setActiveTexture(sprites);
    m_window.draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
}
void UserInterface::drawPanelsFrames(const std::vector<RectangleI>& rects)
{
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;

    constexpr int frameThickness = 7;
    constexpr int numberOfFrameSprites = 8;
    constexpr int baseGradientSpriteX = 690;
    constexpr int baseGradientSpriteY = 0;

    sf::Vector2f frameSpritePositions[numberOfFrameSprites] = {sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY),
                                                               sf::Vector2f(baseGradientSpriteX + frameThickness, baseGradientSpriteY),
                                                               sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY + frameThickness),
                                                               sf::Vector2f(baseGradientSpriteX + frameThickness, baseGradientSpriteY + frameThickness),

                                                               sf::Vector2f(baseGradientSpriteX, baseGradientSpriteY + 2 * frameThickness),
                                                               sf::Vector2f(baseGradientSpriteX + frameThickness, baseGradientSpriteY + 2 * frameThickness),
                                                               sf::Vector2f(baseGradientSpriteX + 2 * frameThickness, baseGradientSpriteY),
                                                               sf::Vector2f(baseGradientSpriteX + 2 * frameThickness, baseGradientSpriteY + frameThickness)
                                                              };
    sf::Vector2f frameSpriteSizes[numberOfFrameSprites] = {sf::Vector2f(frameThickness, frameThickness),
                                                           sf::Vector2f(frameThickness, frameThickness),
                                                           sf::Vector2f(frameThickness, frameThickness),
                                                           sf::Vector2f(frameThickness, frameThickness),

                                                           sf::Vector2f(frameThickness, 436),
                                                           sf::Vector2f(frameThickness, 436),
                                                           sf::Vector2f(334, frameThickness),
                                                           sf::Vector2f(334, frameThickness)
                                                          };

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for(const auto& rect : rects)
    {
        sf::Vector2f framePositions[numberOfFrameSprites] = {sf::Vector2f(rect.min.x, rect.min.y),                                    //top left
                                                             sf::Vector2f(rect.max.x - frameThickness, rect.min.y),                   //top right
                                                             sf::Vector2f(rect.min.x, rect.max.y - frameThickness),                   //bottom left
                                                             sf::Vector2f(rect.max.x - frameThickness, rect.max.y - frameThickness),  //bottom right

                                                             sf::Vector2f(rect.min.x, rect.min.y + frameThickness),                   //left
                                                             sf::Vector2f(rect.max.x - frameThickness, rect.min.y + frameThickness),  //right
                                                             sf::Vector2f(rect.min.x + frameThickness, rect.min.y),                   //top
                                                             sf::Vector2f(rect.min.x + frameThickness, rect.max.y - frameThickness)   //down
                                                            };
        sf::Vector2f frameSizes[numberOfFrameSprites] = {sf::Vector2f(frameThickness, frameThickness),
                                                         sf::Vector2f(frameThickness, frameThickness),
                                                         sf::Vector2f(frameThickness, frameThickness),
                                                         sf::Vector2f(frameThickness, frameThickness),

                                                         sf::Vector2f(frameThickness, rect.height() - 2 * frameThickness),
                                                         sf::Vector2f(frameThickness, rect.height() - 2 * frameThickness),
                                                         sf::Vector2f(rect.width() - 2 * frameThickness, frameThickness),
                                                         sf::Vector2f(rect.width() - 2 * frameThickness, frameThickness),
                                                        };
        for(int i = 0; i < numberOfFrameSprites; ++i)
        {
            sf::Vector2f& position = framePositions[i];
            sf::Vector2f& size = frameSizes[i];
            sf::Vector2f& spritePosition = frameSpritePositions[i];
            sf::Vector2f& spriteSize = frameSpriteSizes[i];

            SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, spriteSize);
        }
    }

    Root::instance().setActiveTexture(sprites);
    m_window.draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
}
void UserInterface::drawReactorBars()
{
    const sf::Texture& barSegmentSprites = Root::instance().resources().textures.barSegments;
    const sf::Texture& moneyBackground = Root::instance().resources().textures.moneyBackground;
    Reactor& reactor = m_game.reactor();
    float approximateWidth = m_window.getSize().x - 200;
    Vec2F center = Vec2F {m_window.getSize().x / 2.0f, 56};
    /* money bar */
    const int height = 32;
    constexpr int frameThickness = 2;
    int allBarsWidth = segmentedBarsWidth(RectangleI(Vec2I {0, 0}, 11, 32), approximateWidth);

    sf::RectangleShape moneyBackgroundFrame;
    moneyBackgroundFrame.setPosition(sf::Vector2f(center.x - allBarsWidth / 2 - frameThickness, center.y + 40.0f * 0 - height / 2 - frameThickness));
    moneyBackgroundFrame.setSize(sf::Vector2f(allBarsWidth + frameThickness * 2, height + frameThickness * 2));
    moneyBackgroundFrame.setFillColor(sf::Color(0, 100, 0));
    m_window.draw(moneyBackgroundFrame, Root::instance().renderStates());

    sf::Sprite moneyBackgroundSprite;
    moneyBackgroundSprite.setTexture(moneyBackground);
    moneyBackgroundSprite.setPosition(sf::Vector2f(center.x - allBarsWidth / 2, center.y + 40.0f * 0 - height / 2));
    moneyBackgroundSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(allBarsWidth, height)));
    m_window.draw(moneyBackgroundSprite, Root::instance().renderStates());

    float heatPercentage = reactor.heatPercentage();
    drawSegmentedBar(barSegmentSprites, RectangleI(Vec2I {0, 0}, 11, 32), RectangleI(Vec2I {0, 32}, 11, 32), center + Vec2F {0.0f, 40.0f * 1}, approximateWidth, heatPercentage, TextRenderer::formatFancy(reactor.storedHeat()) + " / " + TextRenderer::formatFancy(reactor.heatCapacity()));

    float powerPercentage = reactor.powerPercentage();
    drawSegmentedBar(barSegmentSprites, RectangleI(Vec2I {0, 64}, 11, 32), RectangleI(Vec2I {0, 96}, 11, 32), center + Vec2F {0.0f, 40.0f * 2}, approximateWidth, powerPercentage, TextRenderer::formatFancy(reactor.storedPower()) + " / " + TextRenderer::formatFancy(reactor.powerCapacity()));

    TextRenderer::instance().drawFancyNumber("$ " + TextRenderer::formatFancy(m_game.playerStats().money()), m_window, Root::instance().renderStates(), center, TextRenderer::Alignment::Center);
}
int UserInterface::segmentedBarsWidth(const RectangleI& textureRect, float width) const
{
    int segmentWidth = textureRect.width();
    int numberOfSegments = std::round(width / segmentWidth);
    return numberOfSegments * segmentWidth + 2;
}
void UserInterface::drawSegmentedBar(const sf::Texture& texture, const RectangleI& unlitTexture, const RectangleI& litTexture, const Vec2F& center, float width, float litPercentage, const std::string& text)
{
    if(litPercentage > 1.0f) litPercentage = 1.0f;
    else if(litPercentage < 0.0f) litPercentage = 0.0f;
    int segmentWidth = unlitTexture.width();
    int segmentHeight = unlitTexture.height();
    int numberOfSegments = std::round(width / segmentWidth);
    int newWidth = segmentedBarsWidth(unlitTexture, width);
    int numberOfLitSegments = std::round(numberOfSegments * litPercentage);
    int firstLitX = center.x - newWidth / 2;
    int firstUnlitX = firstLitX + numberOfLitSegments * segmentWidth;

    sf::Sprite litSegments;
    litSegments.setTexture(texture);
    litSegments.setPosition(sf::Vector2f(firstLitX, center.y - segmentHeight / 2));
    litSegments.setTextureRect(sf::IntRect(sf::Vector2i(litTexture.min.x - 1, litTexture.min.y), sf::Vector2i(firstUnlitX - firstLitX, segmentHeight)));
    m_window.draw(litSegments, Root::instance().renderStates());

    sf::Sprite unlitSegments;
    unlitSegments.setTexture(texture);
    unlitSegments.setPosition(sf::Vector2f(firstUnlitX, center.y - segmentHeight / 2));
    unlitSegments.setTextureRect(sf::IntRect(sf::Vector2i(unlitTexture.min.x - 1, unlitTexture.min.y), sf::Vector2i(firstLitX + newWidth - firstUnlitX, segmentHeight)));
    m_window.draw(unlitSegments, Root::instance().renderStates());

    TextRenderer::instance().drawFancyNumber(text, m_window, Root::instance().renderStates(), center, TextRenderer::Alignment::Center);
}

void UserInterface::drawWalls(const std::vector<RectangleI>& sceneWalls)
{
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;
    std::vector<UIWall> walls;
    for(const auto& wallRect : sceneWalls)
    {
        walls.emplace_back(wallRect);
    }

    Root::instance().setActiveShader(Root::instance().resources().shaders.worldSpaceNoiseMask);

    constexpr int wallSpritesOffset = 448;
    constexpr int wallSpriteSize = 24;
    constexpr int wallEndSpriteSize = 8;
    std::vector<std::pair<std::reference_wrapper<const UIWall>, std::pair<int, int>>> horizontalWalls; //pair stores minimal excess for two sides
    std::vector<std::pair<std::reference_wrapper<const UIWall>, std::pair<int, int>>> verticalWalls;
    for(const auto& wall : walls)
    {
        if(wall.orientation() == UIWall::Orientation::Horizontal) horizontalWalls.push_back(std::make_pair(std::cref(wall), std::make_pair(wallEndSpriteSize, wallEndSpriteSize))); //initial excess is 8 because more doesn't matter
        else verticalWalls.push_back(std::make_pair(std::cref(wall), std::make_pair(wallEndSpriteSize, wallEndSpriteSize)));
    }
    for(const auto& wall : walls)
    {
        sf::Vector2f size(wall.width(), wall.height());
        sf::RectangleShape shape(size);
        shape.setTexture(&sprites);
        Vec2I textureCoords {wallSpritesOffset, wallSpriteSize};
        if(wall.orientation() == UIWall::Orientation::Vertical) textureCoords.x += wallSpriteSize;
        shape.setTextureRect(sf::IntRect(textureCoords.x, textureCoords.y, wallSpriteSize, wallSpriteSize));
        shape.setPosition(wall.topLeft().x, wall.topLeft().y);
        shape.setFillColor(sf::Color::White);
        m_window.draw(shape, Root::instance().renderStates());
    }
    for(auto& h : horizontalWalls)
    {
        const UIWall& hWall = h.first.get();
        std::pair<int, int>& hExcess = h.second;
        for(auto& v : verticalWalls)
        {
            const UIWall& vWall = v.first.get();
            std::pair<int, int>& vExcess = v.second;
            if(hWall.intersects(vWall))
            {
                int intersectionSpriteId = 0;
                UIWall::UIWallIntersection intersection = hWall.intersection(vWall);
                int leftExcess = intersection.leftExcess;
                int rightExcess = intersection.rightExcess;
                int topExcess = intersection.topExcess;
                int bottomExcess = intersection.bottomExcess;
                hExcess.first = std::min(hExcess.first, leftExcess);
                hExcess.second = std::min(hExcess.second, rightExcess);
                vExcess.first = std::min(vExcess.first, topExcess);
                vExcess.second = std::min(vExcess.second, bottomExcess);
                if(leftExcess <= 0) intersectionSpriteId = 1;
                if(rightExcess <= 0) intersectionSpriteId = 2;
                if(topExcess <= 0) intersectionSpriteId = 3;
                if(bottomExcess <= 0) intersectionSpriteId = 4;

                if(leftExcess <= 0 && topExcess <= 0) intersectionSpriteId = 5;
                if(rightExcess <= 0 && topExcess <= 0) intersectionSpriteId = 6;
                if(bottomExcess <= 0 && rightExcess <= 0) intersectionSpriteId = 7;
                if(bottomExcess <= 0 && leftExcess <= 0) intersectionSpriteId = 8;

                sf::RectangleShape shape(sf::Vector2f(wallSpriteSize, wallSpriteSize));
                shape.setTexture(&sprites);
                shape.setTextureRect(sf::IntRect(wallSpritesOffset + wallSpriteSize * intersectionSpriteId, 0, wallSpriteSize, wallSpriteSize));
                shape.setPosition(intersection.topLeft.x, intersection.topLeft.y);
                shape.setFillColor(sf::Color::White);
                m_window.draw(shape, Root::instance().renderStates());
            }
        }
    }
    for(const auto& h : horizontalWalls)
    {
        const UIWall& wall = h.first.get();
        if(h.second.first >= wallEndSpriteSize)
        {
            sf::RectangleShape shape(sf::Vector2f(wallEndSpriteSize, wallSpriteSize));
            shape.setTexture(&sprites);
            shape.setFillColor(sf::Color::White);
            shape.setTextureRect(sf::IntRect(wallSpritesOffset, wallSpriteSize * 2, wallEndSpriteSize, wallSpriteSize));
            shape.setPosition(wall.topLeft().x, wall.topLeft().y);
            m_window.draw(shape);
        }
        if(h.second.second >= wallEndSpriteSize)
        {
            sf::RectangleShape shape(sf::Vector2f(wallEndSpriteSize, wallSpriteSize));
            shape.setTexture(&sprites);
            shape.setFillColor(sf::Color::White);
            shape.setTextureRect(sf::IntRect(wallSpritesOffset + wallEndSpriteSize, wallSpriteSize * 2, wallEndSpriteSize, wallSpriteSize));
            shape.setPosition(wall.topLeft().x + wall.size() - wallEndSpriteSize, wall.topLeft().y);
            m_window.draw(shape, Root::instance().renderStates());
        }
    }
    return;
    for(const auto& v : verticalWalls)
    {
        const UIWall& wall = v.first.get();
        if(v.second.first >= wallEndSpriteSize)
        {
            sf::RectangleShape shape(sf::Vector2f(wallSpriteSize, wallEndSpriteSize));
            shape.setTexture(&sprites);
            shape.setFillColor(sf::Color::White);
            shape.setTextureRect(sf::IntRect(wallSpritesOffset + wallEndSpriteSize * 2, wallSpriteSize * 2, wallSpriteSize, wallEndSpriteSize));
            shape.setPosition(wall.topLeft().x, wall.topLeft().y);
            m_window.draw(shape, Root::instance().renderStates());
        }
        if(v.second.second >= wallEndSpriteSize)
        {
            sf::RectangleShape shape(sf::Vector2f(wallSpriteSize, wallEndSpriteSize));
            shape.setTexture(&sprites);
            shape.setFillColor(sf::Color::White);
            shape.setTextureRect(sf::IntRect(wallSpritesOffset + wallEndSpriteSize * 2, wallSpriteSize * 2 + wallEndSpriteSize, wallSpriteSize, wallEndSpriteSize));
            shape.setPosition(wall.topLeft().x, wall.topLeft().y + wall.size() - wallEndSpriteSize);
            m_window.draw(shape, Root::instance().renderStates());
        }
    }

    Root::instance().unsetActiveShader();
}
void UserInterface::drawMainScenePanels()
{
    WindowSpaceManager& windowSpaceManager = m_game.windowSpaceManager();
    std::vector<RectangleI> panels;
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::BottomPanel));
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::TopPanel));
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::LeftPanel));
    panels.push_back(windowSpaceManager.windowRegionRect(WindowSpaceManager::WindowRegionId::RightPanel));
    drawPanels(panels);
}

void UserInterface::onWindowResized(sf::Event& windowResizedEvent)
{
}

void UserInterface::onMouseButtonPressed(const Vec2I& windowCoords)
{
    const SegmentStructure& sceneControls = m_game.windowSpaceManager().sceneControls();
    const Vec2I& sceneControlsOffset = m_game.windowSpaceManager().sceneControlsOffset();
    RectangleI sceneControlsRect(sceneControlsOffset, sceneControls.width(), sceneControls.height());
    if(sceneControlsRect.intersects(windowCoords))
    {
        const Segment* pointedSegment = sceneControls.pointedSegment(windowCoords - sceneControlsOffset);
        if(pointedSegment != nullptr)
        {
            clickSegment(*pointedSegment);
        }
    }
}
void UserInterface::clickSegment(const Segment& segment)
{
    const std::string& segmentName = segment.name();
    if(segmentName == "ReactorUnselected")
    {
        m_game.windowSpaceManager().setCurrentScene(WindowSpaceManager::SceneId::Main);
    }
    else if(segmentName == "UpgradesUnselected")
    {
        m_game.windowSpaceManager().setCurrentScene(WindowSpaceManager::SceneId::Upgrades);
    }
    else if(segmentName == "ReactorOn")
    {
        m_game.disableReactor();
    }
    else if(segmentName == "ReactorOff")
    {
        m_game.enableReactor();
    }
    else if(segmentName == "AutoReplaceCellsOn")
    {
        m_game.disableAutoCellReplacing();
    }
    else if(segmentName == "AutoReplaceCellsOff")
    {
        m_game.enableAutoCellReplacing();
    }
    else if(segmentName == "AutoReplaceNeutronReflectorsOn")
    {
        m_game.disableAutoNeutronReflectorReplacing();
    }
    else if(segmentName == "AutoReplaceNeutronReflectorsOff")
    {
        m_game.enableAutoNeutronReflectorReplacing();
    }
}
