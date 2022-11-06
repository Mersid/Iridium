#include <chrono>
#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Material.h"
#include "geometry/Sphere.h"
#include "yaml-cpp/yaml.h" // https://stackoverflow.com/questions/47389809/yaml-cpp-unresolved-external-symbol-error
#include "misc/Vector3dConvert.h"

Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	YAML::Node sceneDef = YAML::LoadFile("data/scene.yml");
	options = std::make_unique<Options>(Options::deserialize(sceneDef["options"]));

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	Scene testScene = Scene::deserialize(sceneDef);
	Texture texture = testScene.render(options->getCameraMode(), options->getRayBounces());
	textureSerializer.serialize(texture, options->getSavePath());

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	std::cout << "Rendered in " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count()) + " seconds" << std::endl;

}

Shimmerlight* Shimmerlight::getInstance()
{
	return instance;
}

TextureSerializer& Shimmerlight::getTextureSerializer()
{
	return textureSerializer;
}

OffSerializer Shimmerlight::getOffSerializer()
{
	return offSerializer;
}

const std::unique_ptr<Options>& Shimmerlight::getOptions() const
{
	return options;
}
