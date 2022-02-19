#include "TextureSerializer.h"
#include "StbImplementation.h"

void TextureSerializer::serialize(Texture& texture, const std::string& file)
{
	StbImplementation::writePNG(file, texture.getWidth(), texture.getHeight(), texture.getData());
}
