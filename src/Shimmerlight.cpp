#include <chrono>
#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Material.h"



Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	Camera camera(800, 800);
	camera.setFov(25);

	Scene defaultScene;
	defaultScene.setCamera(camera);
	Eigen::Vector3d defaultDiffuse(0.4, 0.57, 0.4);
	Eigen::Vector3d defaultSpecular(0.2, 0.2, 0.2);
	double defaultPhongExponent = 256;
	double defaultReflection = 0.7;
	double zOffset = -4.5;
	double defaultLightIntensity = 0.3;
	Material defaultMaterial(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection);

	defaultScene.addLight(Light(Eigen::Vector3d(8, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(6, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(4, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(2, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(0, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-2, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-4, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-5, 0, zOffset), defaultLightIntensity));

	Mesh mesh = offSerializer.loadOff("data/bunny.off");
	Model model(mesh, defaultMaterial);

	model.setPosition(Eigen::Vector3d(0, 0, -5 + zOffset));
	model.setRotation(Eigen::Vector3d(0, 90, 0));
	defaultScene.addModel(model);

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	Texture defaultTextureRender = defaultScene.render();
	textureSerializer.serialize(defaultTextureRender, "default.png");

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
