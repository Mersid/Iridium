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
#include "testing/Derived.h"
#include "testing/Base.h"


Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	YAML::Node sceneDef = YAML::LoadFile("data/Defs/SceneDef.yml");

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	Scene testScene = Scene::deserialize(sceneDef);
	Texture deftexture = testScene.render();
	textureSerializer.serialize(deftexture, "abcde.png");

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
