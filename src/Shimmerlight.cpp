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
	PerlinTextureGenerator perlinTextureGenerator(PerlinFilterMode::CUBIC);
	Texture perlin = perlinTextureGenerator.generateTexture(800, 800, 0.02);
	PerlinTextureGenerator perlinTextureGeneratorLinear(PerlinFilterMode::LINEAR);
	Texture perlinLinear = perlinTextureGeneratorLinear.generateTexture(800, 800, 0.02);
	PerlinTextureGenerator perlinTextureGeneratorFade(PerlinFilterMode::FADE);
	Texture perlinFade = perlinTextureGeneratorFade.generateTexture(800, 800, 0.02);

	CheckerboardTextureGenerator checkerboardTextureGenerator;
	Texture checkerboard = checkerboardTextureGenerator.generateTexture(800, 800, 100);
	Texture worldMap = textureSerializer.deserialize("../world-map.png");

	// 0.006 is a good aperture radius, probably, with 8 rays.
	Camera camera(1600, 800);
	camera.setFov(65);

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
	defaultScene.addPrimitive(std::make_shared<Sphere>( // This sphere is the one to apply the texture to
			Eigen::Vector3d(-2, 0.4, 1 + zOffset), 1,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection, perlin)));
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

	Camera highFOV(1600, 800);
	highFOV.setFov(120);
	defaultScene.setCamera(highFOV);

	Texture highFovTexture = defaultScene.takeSnapshot();
	textureSerializer.serialize(highFovTexture, "high_fov.png");

	Camera lowFOV(1600, 800);
	lowFOV.setFov(30);
	defaultScene.setCamera(lowFOV);

	Texture lowFovTexture = defaultScene.takeSnapshot();
	textureSerializer.serialize(lowFovTexture, "low_fov.png");

	Camera dofCamera(1600, 800, 1, 0.0042, 16);
	dofCamera.setFov(65);
	defaultScene.setCamera(dofCamera);

	Texture dofTexture = defaultScene.takeSnapshot();
	textureSerializer.serialize(dofTexture, "depth_of_field.png");


	defaultScene.setCamera(camera);

	defaultScene.addPrimitive(std::make_shared<Sphere>( // This sphere is the one to apply the texture to
			Eigen::Vector3d(-2, 0.4, 1 + zOffset), 1.001,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection, perlinLinear)));
	Texture defaultTexture2 = defaultScene.takeSnapshot();
	textureSerializer.serialize(defaultTexture2, "linear.png");

	defaultScene.addPrimitive(std::make_shared<Sphere>( // This sphere is the one to apply the texture to
			Eigen::Vector3d(-2, 0.4, 1 + zOffset), 1.002,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection, perlinFade)));
	Texture defaultTexture3 = defaultScene.takeSnapshot();
	textureSerializer.serialize(defaultTexture3, "fade.png");

	defaultScene.addPrimitive(std::make_shared<Sphere>( // This sphere is the one to apply the texture to
			Eigen::Vector3d(-2, 0.4, 1 + zOffset), 1.003,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection, checkerboard)));
	defaultScene.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(1, 0.2, -1 + zOffset), 1.003,
			Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection, worldMap)));
	Texture defaultTexture4 = defaultScene.takeSnapshot();
	textureSerializer.serialize(defaultTexture4, "otherfx.png");

}

Shimmerlight* Shimmerlight::getInstance()
{
	return instance;
}

TextureSerializer& Shimmerlight::getTextureSerializer()
{
	return textureSerializer;
}
