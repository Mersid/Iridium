#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Sphere.h"
#include "noise/PerlinNoiseSampler.h"
#include "geometry/Material.h"


Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	Texture sample(192, 192);
	for (int i = 0; i < 192; i++)
	for (int j = 0; j < 192; j++)
		sample.setPixel(i, j, i + 64, j + 64, 0);


	textureSerializer.serialize(sample, "colortest.png");

	PerlinNoiseSampler perlinNoiseSampler;
	Texture perlin(800, 800);
	for (int i = 0; i < perlin.getHeight() * perlin.getWidth(); i++)
	{
		unsigned int pixelX = i % perlin.getWidth();
		unsigned int pixelY = i / perlin.getWidth();
		unsigned char color = (unsigned char)(255 * std::clamp(perlinNoiseSampler.sample((pixelX + 0.0) * 0.02, (pixelY + 0.0) * 0.02) + 0.5, 0.0, 1.0));
		perlin.setPixel(pixelX, pixelY, 0, color, 0);
	}

	Texture checkerboard(800, 800);
	for (int i = 0; i < checkerboard.getHeight(); i++)
	for (int j = 0; j < checkerboard.getWidth(); j++)
	{
		bool c = false;
		if ((i / 100) % 2 == 0) c = !c;
		if ((j / 100) % 2 == 0) c = !c;

		if (c) checkerboard.setPixel(j, i, 255, 255, 255);
		else checkerboard.setPixel(j, i, 0, 0, 0);
	}
	textureSerializer.serialize(checkerboard, "checkerboard.png");


	Camera camera(800, 800, 1.0, 0.5, 80);

	Scene defaultScene; // TODO: Set ambient?
	defaultScene.setCamera(camera);
	Eigen::Vector3d defaultDiffuse(0.6, 0.4, 0.4);
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


//	Scene customTestScene;
//	customTestScene.setCamera(camera);
//	customTestScene.addLight(Light(Eigen::Vector3d(-2, 4, -5), 1));
//	customTestScene.addPrimitive(std::make_shared<Parallelogram>( // Baseplate
//			Eigen::Vector3d(-10, -2, 0),
//			Eigen::Vector3d(10,-2,0),
//			Eigen::Vector3d(-10,-2,-20)));
//	customTestScene.addPrimitive(std::make_shared<Sphere>( // Blue sphere
//			Eigen::Vector3d(-2, -1, -8), 1,
//			Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	customTestScene.addPrimitive(std::make_shared<Sphere>( // Red sphere
//			Eigen::Vector3d(-2, 1, -8), 1,
//			Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	customTestScene.addPrimitive(std::make_shared<Sphere>( // Floating green sphere
//			Eigen::Vector3d(0, 0.5, -6), 0.5,
//			Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	customTestScene.addPrimitive(std::make_shared<Sphere>( // Near sphere
//			Eigen::Vector3d(1, 0, -4), 0.5,
//			Eigen::Vector3d(0.5, 0.5, 0.5),
//			Eigen::Vector3d(0.5, 0.5, 0.5),
//			10, 0.5));
//	customTestScene.addPrimitive(std::make_shared<Parallelogram>( // Cyan side-facing rectangle
//			Eigen::Vector3d(3, -2, -8),
//			Eigen::Vector3d(5,-2,-6),
//			Eigen::Vector3d(3,-0,-8),
//			Eigen::Vector3d(0, 1.5, 1.5)));
//	Texture multi2 = customTestScene.takeSnapshot();
//	textureSerializer.serialize(multi2, "custom_test.png");


//	Scene parallelogramGreyscale;
//	parallelogramGreyscale.setCamera(camera);
//	parallelogramGreyscale.addPrimitive(std::make_shared<Parallelogram>(
//			Eigen::Vector3d(-0.5, -0.5, -1),
//			Eigen::Vector3d(-0.5, -0.5, -1) + Eigen::Vector3d(0, 0.7, -10),
//			Eigen::Vector3d(-0.5, -0.5, -1) + Eigen::Vector3d(1, 0.4, 0)));
//	parallelogramGreyscale.addLight(Light(Eigen::Vector3d(-1, 0.5, 0), 2));
//
//	Texture planePerspective = parallelogramGreyscale.takeSnapshot(CameraMode::PERSPECTIVE);
//	textureSerializer.serialize(planePerspective, "plane_perspective.png");
//
//	Texture planeOrthographic = parallelogramGreyscale.takeSnapshot(CameraMode::ORTHOGRAPHIC);
//	textureSerializer.serialize(planeOrthographic, "plane_orthographic.png");
//
//
//	Scene sphereGreyscale;
//	sphereGreyscale.setCamera(camera);
//	sphereGreyscale.addPrimitive(std::make_shared<Sphere>(
//			Eigen::Vector3d(0, -0.5, -2.5), 1,
//			Eigen::Vector3d(1.0, 1.0, 1.0), Eigen::Vector3d(0.05, 0.05, 0.05), 3));
//	sphereGreyscale.addLight(Light(Eigen::Vector3d(-0.5, 0.25, -1.5), 1));
//
//	Texture sphere = sphereGreyscale.takeSnapshot(CameraMode::PERSPECTIVE);
//	textureSerializer.serialize(sphere, "sphere.png");
//
//
//	Scene shading;
//	shading.setCamera(camera);
//	shading.addPrimitive(std::make_shared<Sphere>(
//			Eigen::Vector3d(0, -0.5, -2.5), 1,
//			Eigen::Vector3d(1.0, 0.07, 1.0), Eigen::Vector3d(0.05, 0.05, 0.3), 70));
//	shading.addLight(Light(Eigen::Vector3d(-1.6, 1, -1.35), 1));
//
//	Texture shadedSphere = shading.takeSnapshot(CameraMode::PERSPECTIVE);
//	textureSerializer.serialize(shadedSphere, "shading.png");
//
//	Texture shadedSphereOrthographic = shading.takeSnapshot(CameraMode::ORTHOGRAPHIC);
//	textureSerializer.serialize(shadedSphereOrthographic, "shading_orthographic.png");
//
//
//	Scene spherePlaneIntersect;
//
//	spherePlaneIntersect.setCamera(camera);
//	spherePlaneIntersect.addLight(Light(Eigen::Vector3d(-1, 4, -6), 1));
//	spherePlaneIntersect.addPrimitive(std::make_shared<Parallelogram>(
//			Eigen::Vector3d(-2, -2, -5),
//			Eigen::Vector3d(2,-2,-5),
//			Eigen::Vector3d(-2,2,-10)));
//	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
//			Eigen::Vector3d(-2, 0, -8), 1,
//			Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
//			Eigen::Vector3d(-2, 1, -8), 1,
//			Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
//			Eigen::Vector3d(0, 0.5, -6), 0.5,
//			Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	Texture multi = spherePlaneIntersect.takeSnapshot();
//	textureSerializer.serialize(multi, "multi.png");


//	Scene spherePlaneIntersect2;
//
//	spherePlaneIntersect2.setCamera(camera);
//	//spherePlaneIntersect2.addLight(Light(Eigen::Vector3d(-1, 4, -6), 1));
//	spherePlaneIntersect2.addLight(Light(Eigen::Vector3d(-4, 4, -6), 0.65));
//	spherePlaneIntersect2.addLight(Light(Eigen::Vector3d(3, 4, -6), 0.65));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Baseplate
//			Eigen::Vector3d(-10, -2, 0),
//			Eigen::Vector3d(10,-2,0),
//			Eigen::Vector3d(-10,-2,-20)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Blue sphere
//			Eigen::Vector3d(-2, -1, -8), 1,
//			Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Red sphere
//			Eigen::Vector3d(-2, 1, -8), 1,
//			Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Floating green sphere
//			Eigen::Vector3d(0, 0.5, -6), 0.5,
//			Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Bronze sphere
//			Eigen::Vector3d(1, -1.5, -4), 0.5,
//			Eigen::Vector3d(0.5, 0.3, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Yellow sphere w/ blue Phong
//			Eigen::Vector3d(-1, -1.6, -3), 0.4,
//			Eigen::Vector3d(1, 1, 0), Eigen::Vector3d(0, 0, 1)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Lavender sphere
//			Eigen::Vector3d(-3, -1, -4), 0.6,
//			Eigen::Vector3d(0.8, 0.8, 1), Eigen::Vector3d(0.4, 0.4, 0.4)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Pink vertical rectangle
//			Eigen::Vector3d(1, -2, -9),
//			Eigen::Vector3d(3,-2,-9),
//			Eigen::Vector3d(1,-0,-9),
//			Eigen::Vector3d(1, 0.2, 0.6)));
//	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Cyan side-facing rectangle
//			Eigen::Vector3d(3, -2, -8),
//			Eigen::Vector3d(5,-2,-6),
//			Eigen::Vector3d(3,-0,-8),
//			Eigen::Vector3d(0, 1.5, 1.5)));
//	Texture multi2 = spherePlaneIntersect2.takeSnapshot();
//	textureSerializer.serialize(multi2, "multi2.png");
}

Shimmerlight* Shimmerlight::getInstance()
{
	return instance;
}

TextureSerializer& Shimmerlight::getTextureSerializer()
{
	return textureSerializer;
}
