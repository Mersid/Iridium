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
	//defaultScene.addLight(Light(Eigen::Vector3d(0, 0, zOffset), defaultLightIntensity * 4));
	defaultScene.addLight(Light(Eigen::Vector3d(-5, 0, zOffset), defaultLightIntensity));

	Mesh mesh = offSerializer.loadOff("data/cube.off");
	Model model(mesh, defaultMaterial);
	model.translate(Eigen::Vector3d(0, 0, -5 + zOffset));
	model.generateBVH();
	defaultScene.addModel(model);

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
