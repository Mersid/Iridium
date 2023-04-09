#pragma once


#include "geometry/OffSerializer.h"
#include "misc/Options.h"
#include "texture/TextureSerializer.h"

class Iridium
{
public:
	Iridium();
	void run(std::vector<std::string> args);

	static Iridium* getInstance();

	[[nodiscard]] TextureSerializer* getTextureSerializer();
	[[nodiscard]] OffSerializer getOffSerializer();
	[[nodiscard]] const std::unique_ptr<Options>& getOptions() const;


private:
	inline static Iridium* instance;
	std::unique_ptr<TextureSerializer> textureSerializer;
	OffSerializer offSerializer;
	std::unique_ptr<Options> options;
};


