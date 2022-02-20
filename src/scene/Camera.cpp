#include <iostream>
#include "Camera.h"
#include "../texture/Texture.h"
#include "../Shimmerlight.h"
#include "Scene.h" // We can use "Scene.h" here because .cpp files aren't #included, so we won't have to deal with circular dependency

Camera::Camera(int width, int height, double focalLength) :
		width(width), height(height), focalLength(focalLength), aspectRatio((double)width / height)
{
}

Camera::Camera()
{
}

Texture Camera::takeSnapshot(CameraMode cameraMode)
{
	Texture t(width, height);

	for (unsigned int i = 0; i < getPixelCount(); i++)
	{
		Eigen::Vector3d pixelRay = getPixelRayAt(i);
		unsigned int pixelX = i % width;
		unsigned int pixelY = i / width;

		Ray ray;
		if (cameraMode == CameraMode::PERSPECTIVE)
			ray = Ray(Eigen::Vector3d(0, 0, -1 + focalLength), pixelRay); // because plane is at z = -1
		else // Orthographic, so fire rays in a straight line
			// RECALL THAT RAY PARAM TAKES POS AND DIR, NOT START AND END. DON'T BE LIKE ME AND MAKE THIS MISTAKE :'(  - Steven, 2022-02-11
			// Guess what? I made the same mistake again (for the focal length), by changing the aperture's z pos but not the dir. Classic. - Steven, 2022-02-19
			ray = Ray(pixelRay, Eigen::Vector3d(0, 0, -1));

		// Find the nearest primitive we'll hit
		std::shared_ptr<Primitive> primitivePtr = scene->getFirstIntersection(ray);
		Primitive& primitive = *primitivePtr;

		// We hit nothing, then skip this pixel
		if (primitivePtr == nullptr)
			continue;

		std::optional<Eigen::Vector3d> optHitPos = primitive.getRayIntersection(ray);
		if (!optHitPos.has_value()) // Not needed anymore because we refactored above to guarantee primitive hit, but we'll leave it for now
			continue;

		// Compute ray colors
		Eigen::Vector3d hitPos = optHitPos.value();
		Eigen::Vector3d objectNormal = primitive.getNormalAt(hitPos);

		// Handle case of backwards-facing plane. The incoming ray and the normal should face opposite directions.
		// In this case, they aren't, so fix it by changing the normal of the object
		if (objectNormal.dot(ray.getDirection()) > 0)
			objectNormal = -objectNormal;

		// Move hitPos in the normal direction a bit so that the shadow ray won't hit the object itself.
		hitPos = hitPos + objectNormal * 1e-6;

		Light& light = scene->getLight();

		// Unit vector from the impact point to the light
		Eigen::Vector3d lightVector = (light.getPosition() - hitPos).normalized();

		// Specular shading depends on the viewing angle of the user (camera). We need to compute the bisector, from the point of view of the impact position,
		// of the ray to the camera and the vector to the light. The ray's direction vector points from the camera to the impact position, so negative
		// of that points from the impact position to the camera
		Eigen::Vector3d bisector = (-ray.getDirection() + lightVector).normalized();

		// See slide set 5: Final Shading Equation for more on this topic
		// We should note that objectNormal and lightVector are unit vectors, so the dot of them is <= 1

		Eigen::Vector3d ambient = scene->getAmbientCoefficient() * scene->getAmbientLightIntensity();
		Eigen::Vector3d diffuse = primitive.getDiffuseCoefficient() * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));
		Eigen::Vector3d specular = primitive.getSpecularCoefficient() * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getPhongExponent()));

		// If shadow ray hits an object, we won't have light hitting it, so ambient only
		Ray shadow(hitPos, light.getPosition());
		// Ignore self intersection because diffuse/specular dot products will handle it more accurately
		std::shared_ptr<Primitive> shadowHitPtr = scene->getFirstIntersection(shadow, primitivePtr); // TODO: This can *probably* go.
		if (shadowHitPtr != nullptr)
		{
			diffuse = Eigen::Vector3d::Zero();
			specular = Eigen::Vector3d::Zero();
		}

		Eigen::Vector3d color = ambient + diffuse + specular;
		unsigned char colorR = (unsigned char)(255 * std::clamp(color[0], 0.0, 1.0));
		unsigned char colorG = (unsigned char)(255 * std::clamp(color[1], 0.0, 1.0));
		unsigned char colorB = (unsigned char)(255 * std::clamp(color[2], 0.0, 1.0));

		t.setPixel((signed int)pixelX, (signed int)pixelY, colorR, colorG, colorB);
	}
	return t;
}

void Camera::setScene(Scene &s)
{
	this->scene = &s;
}

Eigen::Vector3d Camera::getPixelRayAt(int i)
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

	double halfAlpha = EIGEN_PI / 4; // TODO: Replace this with adjustable focal len

	double pixelCameraX = screenX * aspectRatio * std::tan(halfAlpha);
	double pixelCameraY = screenY * std::tan(halfAlpha);


	return Eigen::Vector3d(pixelCameraX, pixelCameraY, -1); // We're fixing z = -1
}

unsigned int Camera::getPixelCount()
{
	return width * height;
}

