#pragma once


#include "texture/TextureSerializer.h"
#include "geometry/OffSerializer.h"

class Shimmerlight
{
public:
	Shimmerlight();
	void run();

	static Shimmerlight* getInstance();

	[[nodiscard]] TextureSerializer& getTextureSerializer();
	[[nodiscard]] OffSerializer getOffSerializer();

private:
	inline static Shimmerlight* instance;
	TextureSerializer textureSerializer;
	OffSerializer offSerializer;
};


