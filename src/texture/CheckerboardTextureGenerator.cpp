#include "CheckerboardTextureGenerator.h"

Texture CheckerboardTextureGenerator::generateTexture(int width, int height, int squareSize)
{
	Texture checkerboard(width, height);
	for (int i = 0; i < checkerboard.getHeight(); i++)
		for (int j = 0; j < checkerboard.getWidth(); j++)
		{
			bool c = false;
			if ((i / squareSize) % 2 == 0) c = !c;
			if ((j / squareSize) % 2 == 0) c = !c;

			if (c) checkerboard.setPixel(j, i, 255, 255, 255);
			else checkerboard.setPixel(j, i, 0, 0, 0);
		}

	return checkerboard;
}
