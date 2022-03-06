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

	/**
	 * Reads image files into Texture objects
	 * @param file The path to read from. The file MUST be in RGBA format
	 * @return Texture read in. Behaviour is undefined if it does not work
	 */
	Texture deserialize(const std::string& file);
};


