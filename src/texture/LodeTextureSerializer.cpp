#include "LodeTextureSerializer.h"
#include "lodepng.h"

void LodeTextureSerializer::serialize(Texture& texture, const std::string& file)
{
	lodepng::encode(file, texture.getData(), texture.getWidth(), texture.getHeight());
}

