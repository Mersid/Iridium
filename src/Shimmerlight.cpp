#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Sphere.h"
#include "noise/PerlinNoiseSampler.h"
#include "geometry/Material.h"
#include "texture/PerlinTextureGenerator.h"
#include "texture/CheckerboardTextureGenerator.h"


Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	Camera camera(640, 480);

	Scene defaultScene;
	defaultScene.setCamera(camera);
	Eigen::Vector3d defaultDiffuse(0.57, 0.4, 0.4);
	Eigen::Vector3d defaultSpecular(0.2, 0.2, 0.2);
	double defaultPhongExponent = 256;
	double defaultReflection = 0.7;
	double zOffset = -4.5;
	double defaultLightIntensity = 0.3;
	defaultScene.addLight(Light(Eigen::Vector3d(8, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(6, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(4, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(2, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(0, 8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-2, -8, zOffset), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-4, 8, zOffset), defaultLightIntensity));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(10, 0, 1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(7, 0.05, -1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(4, 0.1, 1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(1, 0.2, -1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(-2, 0.4, 1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(-5, 0.8, -1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(-8, 1.6, 1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));
	defaultScene.addPrimitive(std::make_shared<Parallelogram>( // Baseplate
			Eigen::Vector3d(-50, -0.6, 100),
			Eigen::Vector3d(50,-0.6,10),
			Eigen::Vector3d(-50,-0.6,-90),
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection)));

	Texture defaultTextureRender = defaultScene.takeSnapshot();
	textureSerializer.serialize(defaultTextureRender, "default.png");

}

Shimmerlight* Shimmerlight::getInstance()
{
	return instance;
}

TextureSerializer& Shimmerlight::getTextureSerializer()
{
	return textureSerializer;
}
