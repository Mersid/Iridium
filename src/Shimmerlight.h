#pragma once


#include "texture/TextureSerializer.h"
#include "geometry/OffSerializer.h"
#include "misc/Options.h"

class Shimmerlight
{
public:
	Shimmerlight();
	void run();

	static Shimmerlight* getInstance();

	[[nodiscard]] TextureSerializer& getTextureSerializer();
	[[nodiscard]] OffSerializer getOffSerializer();
	[[nodiscard]] const std::unique_ptr<Options>& getOptions() const;

private:
	inline static Shimmerlight* instance;
	TextureSerializer textureSerializer;
	OffSerializer offSerializer;
	std::unique_ptr<Options> options;
};


