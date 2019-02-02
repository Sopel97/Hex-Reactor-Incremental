#include "TextRenderer.h"
#include "Resources.h"
#include "Root.h"

#include "../LibS/Util.h"

#include <algorithm>
#include <iterator>

TextRenderer::TextRenderer()
{
    const char m_fancyNumberCharsetRaw[] = "123456789.!@#:%^&*()KMBTqQsSOND0/abcdefghij$ "; //small digits (after comma) will be !@#$%^&*(; //when i put it as static and then initialize outside class it crashes compiler
    const int m_fancyNumberCharacterSize[] = {14, 16, 17, 18, 17, 17, 17, 17, 17, 9, 13, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 18, 18, 19, 16, 20, 16, 18, 18, 18, 18, 17, 15, 11, 13, 13, 13, 13, 13, 13, 13, 13, 13, 17, 11};
    const int spriteSpacing = 1;
    std::size_t numberOfCharacters = Util::size(m_fancyNumberCharsetRaw);
    int characterSpriteOffset = 0;
    for(unsigned i = 0; i < numberOfCharacters; ++i)
    {
        char character = m_fancyNumberCharsetRaw[i];
        int characterSize = m_fancyNumberCharacterSize[i];

        m_fancyNumberCharset[character] = TextRenderer::CharacterInformation
        {
            Vec2I{characterSpriteOffset, 35},
            Vec2I{characterSpriteOffset, 0},
            Vec2I{characterSize, 35}
        };

        characterSpriteOffset += characterSize + spriteSpacing;
    }
}
TextRenderer& TextRenderer::instance()
{
    static TextRenderer textRenderer;
    return textRenderer;
}

void TextRenderer::preprocessFancyNumberString(std::string& number) const
{
    char smallDigits[256];
    char superscriptDigits[256];
    smallDigits['1'] = '!';
    smallDigits['2'] = '@';
    smallDigits['3'] = '#';
    smallDigits['4'] = ':';
    smallDigits['5'] = '%';
    smallDigits['6'] = '^';
    smallDigits['7'] = '&';
    smallDigits['8'] = '*';
    smallDigits['9'] = '(';
    smallDigits['0'] = ')';

    superscriptDigits['1'] = 'a';
    superscriptDigits['2'] = 'b';
    superscriptDigits['3'] = 'c';
    superscriptDigits['4'] = 'd';
    superscriptDigits['5'] = 'e';
    superscriptDigits['6'] = 'f';
    superscriptDigits['7'] = 'g';
    superscriptDigits['8'] = 'h';
    superscriptDigits['9'] = 'i';
    superscriptDigits['0'] = 'j';

    bool afterComma = false;
    bool afterD = false;
    for(auto& c : number)
    {
        if(c < 0) continue;
        if(c == '.') afterComma = true;
        else if(c == 'D') afterD = true;
        else if(afterD && c >= '0' && c <= '9') c = superscriptDigits[(unsigned)c];
        else if(afterComma && c >= '0' && c <= '9') c = smallDigits[(unsigned)c];
    }
}
Vec2I TextRenderer::calculateTopLeft(const std::string& text, const Vec2F& position, Alignment align) const //text is always centered vertically
{
    constexpr float textHeight = 22.0f;
    if(align == TextRenderer::Alignment::Left)
    {
        return Vec2I(position - Vec2F {0.0f, textHeight / 2.0f});
    }

    Vec2I textSize {0, 0};
    for(const auto& c : text)
    {
        textSize.x += fancyNumberCharacterSize(c).x + m_fancyNumberSpaceBetweenCharacters;
    }

    if(align == TextRenderer::Alignment::Center)
    {
        return Vec2I(position - Vec2F(textSize) * 0.5f - Vec2F {0.0f, textHeight / 2.0f});
    }
    else //Right
    {
        return Vec2I(position - textSize - Vec2F {0.0f, textHeight / 2.0f});
    }
}
void TextRenderer::drawFancyNumber(std::string numberString, sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const Vec2F& position, Alignment align, const sf::Color& color) const
{
    preprocessFancyNumberString(numberString);

    const sf::Texture& texture = Root::instance().resources().textures.font;
    Vec2I topLeft = calculateTopLeft(numberString, position, align);
    topLeft.y += m_fancyNumberTopMargin;
    int offset = 0;

    sf::VertexArray componentVertexArray(sf::PrimitiveType::Quads);
    //background
    for(const auto& c : numberString) //characters
    {
        const Vec2I& spritePos = fancyNumberBackgroundSpritePosition(c);
        const Vec2I& characterSpriteSize = fancyNumberCharacterSize(c);

        sf::Vector2f position(topLeft.x + offset, topLeft.y);
        sf::Vector2f spritePosition(spritePos.x, spritePos.y);
        sf::Vector2f size(characterSpriteSize.x, characterSpriteSize.y);

        componentVertexArray.append(sf::Vertex(position, color, spritePosition));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color, spritePosition + sf::Vector2f(0.0f, size.y)));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(size.x, size.y)));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color, spritePosition + sf::Vector2f(size.x, 0.0f)));

        offset += characterSpriteSize.x + m_fancyNumberSpaceBetweenCharacters;
    }
    offset = 0;
    for(const auto& c : numberString) //characters
    {
        const Vec2I& spritePos = fancyNumberCharacterSpritePosition(c);
        const Vec2I& characterSpriteSize = fancyNumberCharacterSize(c);

        sf::Vector2f position(topLeft.x + offset, topLeft.y);
        sf::Vector2f spritePosition(spritePos.x, spritePos.y);
        sf::Vector2f size(characterSpriteSize.x, characterSpriteSize.y);

        componentVertexArray.append(sf::Vertex(position, color, spritePosition));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color, spritePosition + sf::Vector2f(0.0f, size.y)));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, size.y), color, spritePosition + sf::Vector2f(size.x, size.y)));
        componentVertexArray.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color, spritePosition + sf::Vector2f(size.x, 0.0f)));

        offset += characterSpriteSize.x + m_fancyNumberSpaceBetweenCharacters;
    }

    Root::instance().setActiveTexture(texture);
    renderTarget.draw(componentVertexArray, renderStates);
    Root::instance().unsetActiveTexture();
}
const TextRenderer::CharacterInformation& TextRenderer::characterInformation(char c) const
{
    return m_fancyNumberCharset.at(c);
}
const Vec2I& TextRenderer::fancyNumberCharacterSpritePosition(char c) const
{
    return m_fancyNumberCharset.at(c).characterSpritePosition;
}
const Vec2I& TextRenderer::fancyNumberBackgroundSpritePosition(char c) const
{
    return m_fancyNumberCharset.at(c).backgroundSpritePosition;
}
const Vec2I& TextRenderer::fancyNumberCharacterSize(char c) const
{
    return m_fancyNumberCharset.at(c).characterSize;
}

std::string TextRenderer::formatFancy(const BigInt& baseValue)
{
    if(baseValue == 0) return "0";
    std::string ret;
    std::pair<BigInt, std::string> abbreviations[]
    {
        std::make_pair(BigInt(1e3), std::string("K")),
        std::make_pair(BigInt(1e6), std::string("M")),
        std::make_pair(BigInt(1e9), std::string("B")),
        std::make_pair(BigInt(1e12), std::string("T")),
        std::make_pair(BigInt(1e15), std::string("q"))
    };
    std::reverse(std::begin(abbreviations), std::end(abbreviations));
    std::pair<BigInt, std::string> usedAbbreviation = std::make_pair(BigInt(1), "");
    for(auto begin = std::begin(abbreviations); begin != std::end(abbreviations); ++begin)
    {
        if((*begin).first <= baseValue)
        {
            usedAbbreviation = *begin;
            break;
        }
    }
    std::string baseValueString = std::to_string(baseValue);
    BigInt integer = baseValue / usedAbbreviation.first;
    int intLength = 0;
    BigInt integerCopy = integer;
    while(integerCopy)
    {
        integerCopy /= 10;
        ++intLength;
    }
    ret += std::string(baseValueString, 0, intLength);
    if(intLength < 4)
    {
        if(baseValue >= 1e3)
        {
            ret += ".";
            ret += std::string(baseValueString, intLength, 3);
        }

    }
    if(!usedAbbreviation.second.empty())
    {
        ret += " ";
        ret += usedAbbreviation.second;
    }
    return ret;
}

std::string TextRenderer::formatFancy(float value)
{
    return std::to_string(int((value-1.0f)*100.0f)) + "%";
}

void TextRenderer::drawDescription(const std::string& description, sf::RenderTarget& renderTarger, sf::RenderStates& renderStates, const RectangleI& panelRect, Alignment align) const //needs optimisation, font type and size tweaks, and better position calculation
{
    sf::Text text(sf::String(description), Root::instance().resources().fonts.descriptionFont, 16);

    Vec2I pos{0,0};
    if(align == Alignment::Center) pos = Vec2I(panelRect.min.x + panelRect.width()/2.0f, panelRect.min.y + panelRect.height()/2.0f) - Vec2I(text.getLocalBounds().width, text.getLocalBounds().height + 8)/2;
    else if(align == Alignment::Left) pos = Vec2I(panelRect.min.x, panelRect.min.y + panelRect.height()/2.0f) - Vec2I(0, text.getLocalBounds().height + 8)/2;
    else if(align == Alignment::Right) pos = Vec2I(panelRect.max.x - text.getLocalBounds().width, panelRect.min.y + panelRect.height()/2.0f) - Vec2I(0, text.getLocalBounds().height + 8)/2;

    text.setPosition(sf::Vector2f(pos.x, pos.y) + sf::Vector2f(0.0f, 1.0f));
    text.setColor(sf::Color::Black);
    renderTarger.draw(text, renderStates);

    text.setPosition(sf::Vector2f(pos.x, pos.y) + sf::Vector2f(-1.0f, 0.0f));
    text.setColor(sf::Color::Black);
    renderTarger.draw(text, renderStates);

    text.setPosition(sf::Vector2f(pos.x, pos.y) + sf::Vector2f(0.0f, -1.0f));
    text.setColor(sf::Color::Black);
    renderTarger.draw(text, renderStates);

    text.setPosition(sf::Vector2f(pos.x, pos.y) + sf::Vector2f(1.0f, 0.0f));
    text.setColor(sf::Color::Black);
    renderTarger.draw(text, renderStates);

    text.setPosition(sf::Vector2f(pos.x, pos.y));
    text.setColor(sf::Color::Green + sf::Color(110, 110, 110));
    renderTarger.draw(text, renderStates);
}
