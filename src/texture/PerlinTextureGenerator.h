#pragma once


#include "../noise/PerlinFilterMode.h"
#include "../noise/PerlinNoiseSampler.h"
#include "Texture.h"

class PerlinTextureGenerator
{
public:
	explicit PerlinTextureGenerator(PerlinFilterMode mode);

	/**
	 * @param scale Scale to sample. About 0.02 is a good value.
	 * @return Perlin noise texture with the given size
	 */
	Texture generateTexture(int width, int height, double scale);

private:
	PerlinNoiseSampler sampler;
};


