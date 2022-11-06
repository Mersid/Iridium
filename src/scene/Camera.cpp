#include <iostream>
#include "Camera.h"
#include "../Shimmerlight.h"
#include "Scene.h" // We can use "Scene.h" here because .cpp files aren't #included, so we won't have to deal with circular dependency

Camera::Camera(Transform transform, int width, int height, double focalLength, double apertureRadius, unsigned int rayShots) :
		transform(transform), width(width), height(height), focalLength(focalLength), apertureRadius(apertureRadius),
		rayShots(rayShots), dist(std::uniform_real_distribution<double>(-apertureRadius, apertureRadius)),
		aspectRatio((double)width / height), scene(nullptr)
{
}

Texture Camera::takeSnapshot(CameraMode cameraMode, int ttl)
{
	Texture t(width, height);

	for (unsigned int i = 0; i < getPixelCount(); i++)
	{
		Eigen::Vector3d pixelRay = getPixelRayAt(i);
		unsigned int pixelX = i % width;
		unsigned int pixelY = i / width;

		//if (i % width == 0)
		std::cout << std::to_string(pixelY) + " | " + std::to_string(pixelX) << std::endl;

		// Potentially sample many colors for depth of field, and we need to average them
		std::vector<Eigen::Vector3d> colors;
		colors.reserve(rayShots);

		for (unsigned int j = 0; j < rayShots; j++)
		{
			Ray ray;
			if (cameraMode == CameraMode::PERSPECTIVE)
				ray = Ray(
						Eigen::Vector3d(dist(mt) + transform.getPosition().x(), dist(mt) + transform.getPosition().y(), -1 + focalLength + transform.getPosition().z()),
						pixelRay); // because plane is at z = -1
			else // Orthographic, so fire rays in a straight line
				// RECALL THAT RAY PARAM TAKES POS AND DIR, NOT START AND END. DON'T BE LIKE ME AND MAKE THIS MISTAKE :'(  - Steven, 2022-02-11
				// Guess what? I made the same mistake again (for the focal length), by changing the aperture's z pos but not the dir. Classic. - Steven, 2022-02-19
				ray = Ray(pixelRay + Eigen::Vector3d(0, 0, 1), pixelRay);

			std::optional<Eigen::Vector3d> colorOpt = scene->trace(ray, ttl);
			Eigen::Vector3d color = colorOpt.has_value() ? colorOpt.value() : Eigen::Vector3d::Zero(); // Missed pixels are black
			colors.emplace_back(color);
		}

		double avgR = 0;
		double avgG = 0;
		double avgB = 0;
		for (Eigen::Vector3d& colorVec : colors)
		{
			avgR += colorVec[0];
			avgG += colorVec[1];
			avgB += colorVec[2];
		}
		avgR /= rayShots;
		avgG /= rayShots;
		avgB /= rayShots;

		unsigned char colorR = (unsigned char)(255 * std::clamp(avgR, 0.0, 1.0));
		unsigned char colorG = (unsigned char)(255 * std::clamp(avgG, 0.0, 1.0));
		unsigned char colorB = (unsigned char)(255 * std::clamp(avgB, 0.0, 1.0));

		t.setPixel((signed int)pixelX, (signed int)pixelY, colorR, colorG, colorB);
	}
	return t;
}

void Camera::setScene(Scene &s)
{
	this->scene = &s;
}

Eigen::Vector3d Camera::getPixelRayAt(int i) const
{
	// We start with an empty "image" represented by a vector of "pixel" rays, each pixel of which corresponds to a single pixel on
	// a texture. However, our camera's pixel rays are from -1 to 1, so we need to scale accordingly.
	// We're converting from raster space, to NDC, to screen space for each pixel to evenly distribute our pixel rays.

	unsigned int pixelX = i % width;
	unsigned int pixelY = i / width;

	// NDC, between 0 and 1
	double ndcX = (pixelX + 0.5) / width;
	double ndcY = (pixelY + 0.5) / height;

	// Screen space, which our camera expects. Between -1 and 1 for each axis
	double screenX = 2 * ndcX - 1;
	double screenY = 1 - 2 * ndcY;

	double halfAlpha = std::atan2(1, focalLength);

	double pixelCameraX = screenX * aspectRatio * std::tan(halfAlpha);
	double pixelCameraY = screenY * std::tan(halfAlpha);


	Eigen::Vector3d localPixelRayPos = Eigen::Vector3d(pixelCameraX, pixelCameraY, -1); // We're fixing z = -1

	// We have the local pos, which works fine as-is if the camera never moves (is at origin), but if it moves, we need to
	// apply transformations to it.
	// https://stackoverflow.com/questions/21412169/creating-a-rotation-matrix-with-pitch-yaw-roll-using-eigen

	// Positive numbers rotate up, negative rotates down
	Eigen::AngleAxisd rotX(transform.getRotation().x(), Eigen::Vector3d::UnitX());
	// Positive numbers rotate left, negative rotates right
	Eigen::AngleAxisd rotY(transform.getRotation().y(), Eigen::Vector3d::UnitY());
	// Positive numbers rotate clockwise, negative numbers rotate counterclockwise
	Eigen::AngleAxisd rotZ(transform.getRotation().z(), Eigen::Vector3d::UnitZ());

	localPixelRayPos = rotZ.matrix() * rotY.matrix() * rotX.matrix() * localPixelRayPos;
	localPixelRayPos += transform.getPosition();

	return localPixelRayPos;
}

unsigned int Camera::getPixelCount() const
{
	return width * height;
}

void Camera::setFov(double angle)
{
	// We're manipulating the focal length. When we say, for example, we want 90 degrees vertical FOV, it's actually
	// 45 degrees up and 45 degrees down. For the camera computation, it's easier to assume a bisected triangle from the focal point
	// firing straight into the virtual plane, and then to the top of the plane. Hence, we need to divide the angle by 2.

	// So consider the triangle formed. The vertical is 1 unit, and we can manipulate the focal length f to adjust the FOV.
	// The triangle formed has the angle t = arctan(1/f). We need to isolate f given t.
	// Well, we can obtain tan(t) = 1/f, therefore f = 1/tan(t). Plug in t as the angle (divided by 2 to get only the top half we're considering),
	// and we have f, the focal length, which we can set to adjust the FOV.
	focalLength = 1 / std::tan(angle / 2 * EIGEN_PI / 180);
}

Camera Camera::deserialize(const YAML::Node& node)
{
	auto width = node["width"].as<int>();
	auto height = node["height"].as<int>();
	auto focalLength = node["focalLength"].as<double>();
	auto apertureRadius = node["apertureRadius"].as<double>();
	auto rayShots = node["rayShots"].as<unsigned int>();
	auto transform = Transform::deserialize(node["transform"]);

	Camera camera(transform, width, height, focalLength, apertureRadius, rayShots);
	return camera;
}

const Transform& Camera::getTransform() const
{
	return transform;
}
