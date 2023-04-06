#pragma once


#include "texture/TextureSerializer.h"
#include "geometry/OffSerializer.h"
#include "misc/Options.h"
#include "texture/LodeTextureSerializer.h"

class Iridium
{
public:
	Iridium();
	void run(std::vector<std::string> args);

	static Iridium* getInstance();

	[[nodiscard]] LodeTextureSerializer& getTextureSerializer();
	[[nodiscard]] OffSerializer getOffSerializer();
	[[nodiscard]] const std::unique_ptr<Options>& getOptions() const;

private:
	inline static Iridium* instance;
	LodeTextureSerializer textureSerializer;
	OffSerializer offSerializer;
	std::unique_ptr<Options> options;
};


