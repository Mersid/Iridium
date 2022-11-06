#pragma once


#include "texture/TextureSerializer.h"
#include "geometry/OffSerializer.h"
#include "misc/Options.h"

class Iridium
{
public:
	Iridium();
	void run();

	static Iridium* getInstance();

	[[nodiscard]] TextureSerializer& getTextureSerializer();
	[[nodiscard]] OffSerializer getOffSerializer();
	[[nodiscard]] const std::unique_ptr<Options>& getOptions() const;

private:
	inline static Iridium* instance;
	TextureSerializer textureSerializer;
	OffSerializer offSerializer;
	std::unique_ptr<Options> options;
};


