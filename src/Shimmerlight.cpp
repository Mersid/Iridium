#include <iostream>
#include "Shimmerlight.h"
#include "texture/Texture.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "geometry/Sphere.h"


Shimmerlight::Shimmerlight()
{
	instance = this;
}

void Shimmerlight::run()
{
	Camera camera(1600, 1600);

	Scene parallelogramGreyscale;
	parallelogramGreyscale.setCamera(camera);
	parallelogramGreyscale.addPrimitive(std::make_shared<Parallelogram>(
			Eigen::Vector3d(-0.5, -0.5, -1),
			Eigen::Vector3d(-0.5, -0.5, -1) + Eigen::Vector3d(0, 0.7, -10),
			Eigen::Vector3d(-0.5, -0.5, -1) + Eigen::Vector3d(1, 0.4, 0)));
	parallelogramGreyscale.setLight(Light(Eigen::Vector3d(-1, 0.5, 0), 2));

	Texture planePerspective = parallelogramGreyscale.takeSnapshot(CameraMode::PERSPECTIVE);
	textureSerializer.serialize(planePerspective, "plane_perspective.png");

	Texture planeOrthographic = parallelogramGreyscale.takeSnapshot(CameraMode::ORTHOGRAPHIC);
	textureSerializer.serialize(planeOrthographic, "plane_orthographic.png");


	Scene sphereGreyscale;
	sphereGreyscale.setCamera(camera);
	sphereGreyscale.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(0, -0.5, -2.5), 1,
			Eigen::Vector3d(1.0, 1.0, 1.0), Eigen::Vector3d(0.05, 0.05, 0.05), 3));
	sphereGreyscale.setLight(Light(Eigen::Vector3d(-0.5, 0.25, -1.5), 1));

	Texture sphere = sphereGreyscale.takeSnapshot(CameraMode::PERSPECTIVE);
	textureSerializer.serialize(sphere, "sphere.png");


	Scene shading;
	shading.setCamera(camera);
	shading.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(0, -0.5, -2.5), 1,
			Eigen::Vector3d(1.0, 0.07, 1.0), Eigen::Vector3d(0.05, 0.05, 0.3), 70));
	shading.setLight(Light(Eigen::Vector3d(-1.6, 1, -1.35), 1));

	Texture shadedSphere = shading.takeSnapshot(CameraMode::PERSPECTIVE);
	textureSerializer.serialize(shadedSphere, "shading.png");

	Texture shadedSphereOrthographic = shading.takeSnapshot(CameraMode::ORTHOGRAPHIC);
	textureSerializer.serialize(shadedSphereOrthographic, "shading_orthographic.png");


	Scene spherePlaneIntersect;

	spherePlaneIntersect.setCamera(camera);
	spherePlaneIntersect.setLight(Light(Eigen::Vector3d(-1, 4, -6), 1));
	spherePlaneIntersect.addPrimitive(std::make_shared<Parallelogram>(
			Eigen::Vector3d(-2, -2, -5),
			Eigen::Vector3d(2,-2,-5),
			Eigen::Vector3d(-2,2,-10)));
	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(-2, 0, -8), 1,
			Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(-2, 1, -8), 1,
			Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect.addPrimitive(std::make_shared<Sphere>(
			Eigen::Vector3d(0, 0.5, -6), 0.5,
			Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	Texture multi = spherePlaneIntersect.takeSnapshot();
	textureSerializer.serialize(multi, "multi.png");


	Scene spherePlaneIntersect2;

	spherePlaneIntersect2.setCamera(camera);
	spherePlaneIntersect2.setLight(Light(Eigen::Vector3d(-1, 4, -6), 1));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Baseplate
			Eigen::Vector3d(-10, -2, 0),
			Eigen::Vector3d(10,-2,0),
			Eigen::Vector3d(-10,-2,-20)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Blue sphere
			Eigen::Vector3d(-2, -1, -8), 1,
			Eigen::Vector3d(0.1, 0.1, 0.5), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Red sphere
			Eigen::Vector3d(-2, 1, -8), 1,
			Eigen::Vector3d(0.5, 0.1, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Floating green sphere
			Eigen::Vector3d(0, 0.5, -6), 0.5,
			Eigen::Vector3d(0.1, 0.5, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Bronze sphere
			Eigen::Vector3d(1, -1.5, -4), 0.5,
			Eigen::Vector3d(0.5, 0.3, 0.1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Yellow sphere w/ blue Phong
			Eigen::Vector3d(-1, -1.6, -3), 0.4,
			Eigen::Vector3d(1, 1, 0), Eigen::Vector3d(0, 0, 1)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Sphere>( // Lavender sphere
			Eigen::Vector3d(-3, -1, -4), 0.6,
			Eigen::Vector3d(0.8, 0.8, 1), Eigen::Vector3d(0.4, 0.4, 0.4)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Pink vertical rectangle
			Eigen::Vector3d(1, -2, -9),
			Eigen::Vector3d(3,-2,-9),
			Eigen::Vector3d(1,-0,-9),
			Eigen::Vector3d(1, 0.2, 0.6)));
	spherePlaneIntersect2.addPrimitive(std::make_shared<Parallelogram>( // Cyan side-facing rectangle
			Eigen::Vector3d(3, -2, -8),
			Eigen::Vector3d(5,-2,-6),
			Eigen::Vector3d(3,-0,-8),
			Eigen::Vector3d(0, 1.5, 1.5)));
	Texture multi2 = spherePlaneIntersect2.takeSnapshot();
	textureSerializer.serialize(multi2, "multi2.png");
}

Shimmerlight* Shimmerlight::getInstance()
{
	return instance;
}

TextureSerializer& Shimmerlight::getTextureSerializer()
{
	return textureSerializer;
}
