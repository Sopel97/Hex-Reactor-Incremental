#include "UpgradeInterface.h"
#include "Root.h"
#include "SFMLUtil.h"
#include "Upgrade.h"

SegmentStructure UpgradeInterface::m_segmentStructure
{
    std::vector<Segment>
    {
        Segment{
            "MinusEnabled",
            Vec2F{448.0f, 72.0f},
            Vec2F{0.0f, 26.0f},
            Vec2F{96.0f, 51.0f},
            PolygonF
            {
                Vec2F{25.0f, 0.0f},
                Vec2F{95.0f, 0.0f},
                Vec2F{70.0f, 25.0f},
                Vec2F{95.0f, 50.0f},
                Vec2F{25.0f, 50.0f},
                Vec2F{0.0f, 25.0f}
            }
        },
        Segment{
            "MinusDisabled",
            Vec2F{448.0f, 123.0f},
            Vec2F{0.0f, 26.0f},
            Vec2F{96.0f, 51.0f},
            PolygonF
            {
                Vec2F{25.0f, 0.0f},
                Vec2F{95.0f, 0.0f},
                Vec2F{70.0f, 25.0f},
                Vec2F{95.0f, 50.0f},
                Vec2F{25.0f, 50.0f},
                Vec2F{0.0f, 25.0f}
            }
        },
        Segment{
            "IconDisplay",
            Vec2F{448.0f, 225.0f},
            Vec2F{96.0f, 0.0f},
            Vec2F{90.0f, 52.0f},
            PolygonF
            {
                Vec2F{25.0f, 1.0f},
                Vec2F{64.0f, 1.0f},
                Vec2F{99, 26},
                Vec2F{66, 51}, //TODO: check it it's not 64
                Vec2F{25, 51},
                Vec2F{0, 26}
            }
        },
        Segment{
            "LevelDisplay",
            Vec2F{538.0f, 225.0f},
            Vec2F{71.0f, 27.0f},
            Vec2F{140.0f, 76.0f},
            PolygonF
            {
                Vec2F{24, 0},
                Vec2F{25, 0},
                Vec2F{50, 24},
                Vec2F{89, 24},
                Vec2F{114, 0},
                Vec2F{115, 0},
                Vec2F{139, 24},
                Vec2F{89, 74},
                Vec2F{50, 74},
                Vec2F{0, 24}
            }
        },
        Segment{
            "PlusEnabled",
            Vec2F{544.0f, 72.0f},
            Vec2F{186.0f, 26.0f},
            Vec2F{96.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{70, 0},
                Vec2F{95, 25},
                Vec2F{70, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{
            "PlusDisabled",
            Vec2F{544.0f, 123.0f},
            Vec2F{186.0f, 26.0f},
            Vec2F{96.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{70, 0},
                Vec2F{95, 25},
                Vec2F{70, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{
            "UnlockEnabled",
            Vec2F{448.0f, 174.0f},
            Vec2F{257.0f, 26.0f},
            Vec2F{120.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{94, 0},
                Vec2F{119, 25},
                Vec2F{94, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{
            "UnlockDisabled",
            Vec2F{568.0f, 174.0f},
            Vec2F{257.0f, 26.0f},
            Vec2F{120.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{94, 0},
                Vec2F{119, 25},
                Vec2F{94, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{
            "UnlockMaxed",
            Vec2F{704.0f, 166.0f},
            Vec2F{257.0f, 26.0f},
            Vec2F{120.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{94, 0},
                Vec2F{119, 25},
                Vec2F{94, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{
            "NameDisplay",
            Vec2F{342.0f, 301.0f},
            Vec2F{352.0f, 26.0f},
            Vec2F{348.0f, 51.0f},
            PolygonF
            {
                Vec2F{0, 0},
                Vec2F{322, 0},
                Vec2F{347, 25},
                Vec2F{322, 50},
                Vec2F{0, 50},
                Vec2F{25, 25}
            }
        },
        Segment{ //TODO: check volume of this segment
            "DescriptionDisplay",
            Vec2F{150.0f, 352.0f},
            Vec2F{161.0f, 77.0f},
            Vec2F{539.0f, 100.0f},
            PolygonF
            {
                Vec2F{24, 0},
                Vec2F{513, 0},
                Vec2F{538, 25},
                Vec2F{513, 50},
                Vec2F{538, 75},
                Vec2F{513, 100},
                Vec2F{24, 100},
                Vec2F{0, 75},
                Vec2F{24, 50},
                Vec2F{0, 25}
            }
        }
    }
};

UpgradeInterface::UpgradeInterface(UpgradeBase& upgrade, const Vec2I& offset) :
    m_upgrade(upgrade),
    m_worldOffset(offset)

{
}

const UpgradeBase& UpgradeInterface::upgrade() const
{
    return m_upgrade;
}
const Vec2I& UpgradeInterface::worldOffset() const
{
    return m_worldOffset;
}
void UpgradeInterface::setWorldOffset(const Vec2I& newOffset)
{
    m_worldOffset = newOffset;
}

void UpgradeInterface::updateSegments()
{
    const BigInt& playerMoney = Root::instance().game().playerStats().money();

    m_segmentStructure.disableAllSegments();

    m_segmentStructure.segment("DescriptionDisplay").enable();
    m_segmentStructure.segment("IconDisplay").enable();
    m_segmentStructure.segment("LevelDisplay").enable();
    m_segmentStructure.segment("NameDisplay").enable();

    if(m_upgrade.isLowestPossibleLevelSelected())
    {
        m_segmentStructure.segment("MinusDisabled").enable();
    }
    else
    {
        m_segmentStructure.segment("MinusEnabled").enable();
    }

    if(m_upgrade.isLevelMaxed())
    {
        m_segmentStructure.segment("UnlockMaxed").enable();
    }
    else if(m_upgrade.isNextLevelAffordable(playerMoney))
    {
        m_segmentStructure.segment("UnlockEnabled").enable();
    }
    else
    {
        m_segmentStructure.segment("UnlockDisabled").enable();
    }

    if(m_upgrade.isHighestPossibleLevelSelected())
    {
        m_segmentStructure.segment("PlusDisabled").enable();
    }
    else
    {
        m_segmentStructure.segment("PlusEnabled").enable();
    }
}

void UpgradeInterface::draw(sf::RenderTarget& renderTarget)
{
    updateSegments();
    const sf::Texture& sprites = Root::instance().resources().textures.spritesheet;

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);
    m_segmentStructure.draw(vertexArray, m_worldOffset);

    std::shared_ptr<Component> iconComponent = m_upgrade.iconComponent();
    const Segment& iconSegment = m_segmentStructure.segment("IconDisplay");
    Vec2F size(32.0f, 32.0f);
    Vec2F position = iconSegment.position() + iconSegment.size() / 2.0f - size / 2.0f + m_worldOffset;
    Vec2F spritePosition = iconComponent->spritePosition();

    SFMLUtil::appendQuadAsTriangleList(vertexArray, position, size, spritePosition, size);
    //Root::instance().setActiveShader(Root::instance().resources().shaders.worldSpaceNoiseMask); //i dont know if it's better with or without it
    Root::instance().setActiveTexture(sprites);
    renderTarget.draw(vertexArray, Root::instance().renderStates());
    Root::instance().unsetActiveTexture();
    //Root::instance().unsetActiveShader();

    Root::instance().renderStates().blendMode = sf::BlendAdd; //this along with color set to half white mimics grain extract effect

    sf::Text upgradeName(sf::String(m_upgrade.name()), Root::instance().resources().fonts.standardFont, 17);
    const auto& nameDisplaySegment = m_segmentStructure.segment("NameDisplay");
    upgradeName.setPosition(nameDisplaySegment.position() + nameDisplaySegment.size() / 2.0f + m_worldOffset - Vec2F(upgradeName.getLocalBounds().width / 2.0f, upgradeName.getLocalBounds().height / 2.0f));
    upgradeName.setColor(sf::Color(128, 128, 128));
    renderTarget.draw(upgradeName, Root::instance().renderStates());


    sf::Text upgradeLevel(sf::String(std::to_string(m_upgrade.selectedLevel()) + "/" + std::to_string(m_upgrade.maxUnlockedLevel())), Root::instance().resources().fonts.standardFont, 17);
    const auto& levelDisplaySegment = m_segmentStructure.segment("LevelDisplay");
    upgradeLevel.setPosition(levelDisplaySegment.position() + levelDisplaySegment.size() / 2.0f + m_worldOffset - Vec2F(upgradeLevel.getLocalBounds().width / 2.0f, upgradeLevel.getLocalBounds().height / 2.0f));
    upgradeLevel.setColor(sf::Color(128, 128, 128));
    renderTarget.draw(upgradeLevel, Root::instance().renderStates());

    Root::instance().renderStates().blendMode = sf::BlendAlpha; //stop the effect of grain extract and return to default

    const Segment& descriptionSegment = m_segmentStructure.segment("DescriptionDisplay");
    TextRenderer::instance().drawDescription(m_upgrade.formattedDescription(), renderTarget, Root::instance().renderStates(), RectangleI(m_worldOffset + descriptionSegment.position() + Vec2I {36, 0}, descriptionSegment.size().x, descriptionSegment.size().y), TextRenderer::Alignment::Left);
}
void UpgradeInterface::update(double dt)
{
}
void UpgradeInterface::onMouseButtonPressed(const Vec2I& localCoords)
{
    updateSegments();
    auto clickedSegment = m_segmentStructure.pointedSegment(localCoords);
    if(clickedSegment != nullptr)
    {
        clickSegment(*clickedSegment);
    }
}
void UpgradeInterface::clickSegment(const Segment& segment)
{
    const std::string& segmentName = segment.name();
    if(segmentName == "PlusEnabled")
    {
        m_upgrade.tryIncreaseSelectedLevel();
    }
    else if(segmentName == "MinusEnabled")
    {
        m_upgrade.tryDecreaseSelectedLevel();
    }
    else if(segmentName == "UnlockEnabled")
    {
        m_upgrade.tryUnlockNextLevel();
    }
}
float UpgradeInterface::width()
{
    return m_segmentStructure.width();
}
float UpgradeInterface::height()
{
    return m_segmentStructure.height();
}
