#include "StbTextureSerializer.h"
#include "StbImplementation.h"

void StbTextureSerializer::serialize(Texture& texture, const std::string& file)
{
	StbImplementation::writePNG(file, texture.getWidth(), texture.getHeight(), texture.getData());
}

Texture StbTextureSerializer::deserialize(const std::string& file)
{
	return StbImplementation::readPNG(file);
}
