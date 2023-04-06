#include <chrono>
#include <iostream>
#include "Iridium.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Material.h"
#include "geometry/Sphere.h"
#include "yaml-cpp/yaml.h" // https://stackoverflow.com/questions/47389809/yaml-cpp-unresolved-external-symbol-error
#include "misc/Vector3dConvert.h"

Iridium::Iridium()
{
	instance = this;
}

void Iridium::run(std::vector<std::string> args)
{
	// TODO: What if no file named this? Will always crash even if we specify custom file
	YAML::Node sceneDef;

    if (args.size() > 1)
        sceneDef = YAML::LoadFile(args[1]);
    else
        sceneDef = YAML::LoadFile("data/scene.yml");

    options = std::make_unique<Options>(Options::deserialize(sceneDef["options"]));

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	std::unique_ptr<Scene> testScene = Scene::deserialize(sceneDef);
	Texture texture = testScene->render(options->getCameraMode(), options->getRayBounces());
	textureSerializer.serialize(texture, options->getSavePath());

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	std::cout << "Rendered in " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t1 - t0).count()) + " seconds" << std::endl;

}

Iridium* Iridium::getInstance()
{
	return instance;
}

LodeTextureSerializer& Iridium::getTextureSerializer()
{
	return textureSerializer;
}

OffSerializer Iridium::getOffSerializer()
{
	return offSerializer;
}

const std::unique_ptr<Options>& Iridium::getOptions() const
{
	return options;
}
