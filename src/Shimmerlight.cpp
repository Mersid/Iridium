#include <chrono>
#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Material.h"
#include "geometry/Sphere.h"


Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	Camera camera(800, 800);
	camera.setFov(90);

	Scene defaultScene;
	defaultScene.setCamera(camera);
	Eigen::Vector3d defaultDiffuse(0.35, 0.8, 0.35);
	Eigen::Vector3d defaultSpecular(0.2, 0.2, 0.2);
	double defaultPhongExponent = 256;
	double defaultReflection = 0.7;
	double zOffset = -2;
	double defaultLightIntensity = 0.13;
	Material defaultMaterial(defaultDiffuse, defaultSpecular, defaultPhongExponent, defaultReflection);

	defaultScene.addLight(Light(Eigen::Vector3d(8, 8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(6, -8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(4, 8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(2, -8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(0, 8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-2, -8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-4, 8, 0), defaultLightIntensity));
	defaultScene.addLight(Light(Eigen::Vector3d(-5, 0, 0), defaultLightIntensity));

	Mesh dragonMesh = offSerializer.loadOff("data/dragon.off");
	Model dragonModel(dragonMesh, defaultMaterial);

	dragonModel.setPosition(Eigen::Vector3d(0, 0, zOffset));
	dragonModel.setRotation(Eigen::Vector3d(0, 0, 0));
	dragonModel.setScale(Eigen::Vector3d(3, 3, 3));
	defaultScene.addModel(dragonModel);

	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

	//Texture dragonTexture = defaultScene.render();
	//textureSerializer.serialize(dragonTexture, "dragon.png");


	defaultScene.clearModels();
	dragonModel.setRotation(Eigen::Vector3d(0, 80, 0));
	dragonModel.setScale(Eigen::Vector3d(2, 2, 2));
	defaultScene.addModel(dragonModel);

	//Texture dragonForwardsTexture = defaultScene.render();
	//textureSerializer.serialize(dragonForwardsTexture, "dragon_forwards.png");


	Camera combinedCamera(4000, 4000);

	Scene combinedScene;
	combinedScene.setCamera(combinedCamera);
	combinedScene.addLight(Light(Eigen::Vector3d(-1, 4, -6), 0.5));
	combinedScene.addLight(Light(Eigen::Vector3d(0, 4, 0), 0.5));

	std::vector<std::shared_ptr<Primitive>> worldMeshPrimitives = {
			std::make_shared<Parallelogram>( // Baseplate
					Eigen::Vector3d(-10, -2, 0),
					Eigen::Vector3d(10,-2,0),
					Eigen::Vector3d(-10,-2,-20),
					Material(Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d(0.5, 0.5, 0.5), 10, 0.7)),

			std::make_shared<Sphere>( // Blue sphere
					Eigen::Vector3d(-2, -1, -8), 1,
					Material(Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4))),

			std::make_shared<Sphere>( // Red sphere
					Eigen::Vector3d(-2, 1, -8), 1,
					Material(Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4))),

			std::make_shared<Sphere>( // Floating green sphere
					Eigen::Vector3d(0, 0.5, -6), 0.5,
					Material(Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4), 10, 0.5)),

			std::make_shared<Sphere>( // Bronze sphere
					Eigen::Vector3d(1, -1.5, -4), 0.5,
					Material(Eigen::Vector3d(0.5, 0.3, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4), 12, 0.25)),

			std::make_shared<Sphere>( // Yellow sphere w/ blue Phong
					Eigen::Vector3d(-1, -1.6, -3), 0.4,
					Material(Eigen::Vector3d(1, 1, 0), Eigen::Vector3d(0, 0, 1))),

			std::make_shared<Sphere>( // Lavender sphere
					Eigen::Vector3d(-3, -1, -4), 0.6,
					Material(Eigen::Vector3d(0.8, 0.8, 1), Eigen::Vector3d(0.4, 0.4, 0.4))),

			std::make_shared<Parallelogram>( // Pink vertical rectangle
					Eigen::Vector3d(1, -2, -9),
					Eigen::Vector3d(3,-2,-9),
					Eigen::Vector3d(1,-0,-9),
					Material(Eigen::Vector3d(1, 0.2, 0.6))),

			std::make_shared<Parallelogram>( // Cyan side-facing rectangle
					Eigen::Vector3d(3, -2, -8),
					Eigen::Vector3d(5,-2,-6),
					Eigen::Vector3d(3,-0,-8),
					Material(Eigen::Vector3d(0, 1.5, 1.5)))
	};

	Mesh worldMesh(worldMeshPrimitives);
	Model worldModel(worldMesh, Material(), false);
	combinedScene.addModel(worldModel);

	Model redDragon(dragonMesh, Material(Eigen::Vector3d(0.95, 0.42, 0.42), defaultSpecular, defaultPhongExponent, 0));
	redDragon.setPosition(Eigen::Vector3d(-1.2, -0.15, -5));
	redDragon.setRotation(Eigen::Vector3d(0, 70, 0));
	redDragon.setScale(Eigen::Vector3d(2, 2, 2));
	combinedScene.addModel(redDragon);

	Mesh cubeMesh = offSerializer.loadOff("data/cube.off");
	Model cubeModel(cubeMesh, Material(Eigen::Vector3d(0.95, 0.42, 0.15), defaultSpecular, defaultPhongExponent, 0.55));
	cubeModel.setPosition(Eigen::Vector3d(-2, -2, -5));
	cubeModel.setRotation(Eigen::Vector3d(0, 45, 0));
	cubeModel.setScale(Eigen::Vector3d(1.2, 1.2, 1.2));
	combinedScene.addModel(cubeModel);

	Mesh bunnyMesh = offSerializer.loadOff("data/bunny.off");
	Model bunnyModel(bunnyMesh, Material(defaultDiffuse, defaultSpecular, defaultPhongExponent, 0.15));
	bunnyModel.setPosition(Eigen::Vector3d(1.5, -0.8, -2.6));
	bunnyModel.setRotation(Eigen::Vector3d(10, 45, 0));
	bunnyModel.setScale(Eigen::Vector3d(0.8, 0.8, 0.8));
	combinedScene.addModel(bunnyModel);

	Mesh dodecaMesh = offSerializer.loadOff("data/dodeca.off");
	Model dodecaModel(dodecaMesh, Material(Eigen::Vector3d(0.15, 0.42, 0.95), defaultSpecular, defaultPhongExponent, 0.55));
	dodecaModel.setPosition(Eigen::Vector3d(1, -2, -3));
	dodecaModel.setRotation(Eigen::Vector3d(0, 0, 0));
	dodecaModel.setScale(Eigen::Vector3d(1.0, 1.0, 1.0));
	combinedScene.addModel(dodecaModel);

	Texture combinedTexture = combinedScene.render();
	textureSerializer.serialize(combinedTexture, "combined.png");


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
