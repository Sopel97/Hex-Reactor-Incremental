#include "Resources.h"

Resources::Resources()
{
    textures.grid.loadFromFile("assets\\gfx\\grid.png");

    textures.hexTileMask.loadFromFile("assets\\gfx\\hexTileMask.png");

    textures.spritesheet.loadFromFile("assets\\gfx\\sprites.png");

    textures.uiBackground.loadFromFile("assets\\gfx\\sprites.png", sf::IntRect(sf::Vector2i(824, 14), sf::Vector2i(93, 120)));
    textures.uiBackground.setRepeated(true);

    textures.noise.loadFromFile("assets\\gfx\\noiseMask.png");
    textures.noise.setRepeated(true);

    textures.font.loadFromFile("assets\\gfx\\font.png");

    textures.barSegments.loadFromFile("assets\\gfx\\sprites.png", sf::IntRect(sf::Vector2i(665, 0), sf::Vector2i(11, 128)));
    textures.barSegments.setRepeated(true);

    textures.moneyBackground.loadFromFile("assets\\gfx\\sprites.png", sf::IntRect(sf::Vector2i(704, 134), sf::Vector2i(100, 32)));
    textures.moneyBackground.setRepeated(true);

    textures.explosion.loadFromFile("assets\\gfx\\explosion.png");

    shaders.worldSpaceNoiseMask.loadFromFile("assets\\shaders\\worldSpaceGrainMergeWithNoise.vert", "assets\\shaders\\worldSpaceGrainMergeWithNoise.frag");
    shaders.worldSpaceNoiseMask.setParameter("texture", sf::Shader::CurrentTexture);
    shaders.worldSpaceNoiseMask.setParameter("noiseTexture", textures.noise);
    shaders.worldSpaceNoiseMask.setParameter("noiseTextureSize", textures.noise.getSize().x, textures.noise.getSize().y);

    shaders.screenSpaceNoiseMask.loadFromFile("assets\\shaders\\screenSpaceGrainMergeWithNoise.vert", "assets\\shaders\\screenSpaceGrainMergeWithNoise.frag");
    shaders.screenSpaceNoiseMask.setParameter("texture", sf::Shader::CurrentTexture);
    shaders.screenSpaceNoiseMask.setParameter("noiseTexture", textures.noise);

    fonts.descriptionFont.loadFromFile("assets\\fonts\\descriptionFont.ttf");
    fonts.standardFont.loadFromFile("assets\\fonts\\standardFont.ttf");

}
