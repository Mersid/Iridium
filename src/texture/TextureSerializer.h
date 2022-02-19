#pragma once


#include <string>
#include "Texture.h"

class TextureSerializer
{
public:
	/**
	 * Writes Texture objects to file
	 * @param texture The Texture object to save to file
	 * @param file The file to save the Texture object to, in PNG RGBA format.
	 */
	void serialize(Texture& texture, const std::string& file);
};


