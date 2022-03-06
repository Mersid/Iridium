#include "TextureSerializer.h"
#include "StbImplementation.h"

void TextureSerializer::serialize(Texture& texture, const std::string& file)
{
	StbImplementation::writePNG(file, texture.getWidth(), texture.getHeight(), texture.getData());
}

Texture TextureSerializer::deserialize(const std::string& file)
{
	return StbImplementation::readPNG(file);
}
