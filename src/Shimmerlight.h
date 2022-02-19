#pragma once


#include "texture/TextureSerializer.h"

class Shimmerlight
{
public:
	Shimmerlight();
	void run();

	static Shimmerlight* getInstance();

	[[nodiscard]] TextureSerializer& getTextureSerializer();

private:
	inline static Shimmerlight* instance;
	TextureSerializer textureSerializer;
};


