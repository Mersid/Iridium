#include "PerlinTextureGenerator.h"

PerlinTextureGenerator::PerlinTextureGenerator(PerlinFilterMode mode) : sampler(PerlinNoiseSampler(mode))
{

}

Texture PerlinTextureGenerator::generateTexture(int width, int height, double scale)
{
	Texture perlin(width, height);
	for (int i = 0; i < perlin.getHeight() * perlin.getWidth(); i++)
	{
		unsigned int pixelX = i % perlin.getWidth();
		unsigned int pixelY = i / perlin.getWidth();
		unsigned char color = (unsigned char)(255 * std::clamp(sampler.sample(pixelX * scale, pixelY * scale) + 0.5, 0.0, 1.0));
		perlin.setPixel(pixelX, pixelY, 0, color, 0);
	}

	return perlin;
}
