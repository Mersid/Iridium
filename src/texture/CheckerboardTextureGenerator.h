#pragma once


#include "Texture.h"

class CheckerboardTextureGenerator
{
public:

	/**
	 * @param squareSize Size of the square, in pixels. So, a 800x800 texture with squareSize 100 will have a 8x8 grid.
	 * @return A checkerboard texture, with the top-left being black. Edges on the right and bottom will be truncated if squareSize is not a multiple of width or height
	 */
	Texture generateTexture(int width, int height, int squareSize);

};


