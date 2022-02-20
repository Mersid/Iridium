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

		if (i % width == 0)
			std::cout << std::to_string(pixelY) << std::endl;

		Ray ray;
		if (cameraMode == CameraMode::PERSPECTIVE)
			ray = Ray(Eigen::Vector3d(0, 0, -1 + focalLength), pixelRay); // because plane is at z = -1
		else // Orthographic, so fire rays in a straight line
			// RECALL THAT RAY PARAM TAKES POS AND DIR, NOT START AND END. DON'T BE LIKE ME AND MAKE THIS MISTAKE :'(  - Steven, 2022-02-11
			// Guess what? I made the same mistake again (for the focal length), by changing the aperture's z pos but not the dir. Classic. - Steven, 2022-02-19
			ray = Ray(pixelRay + Eigen::Vector3d(0, 0, 1), pixelRay);

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

		std::vector<Light>& lights = scene->getLights();

		Eigen::Vector3d ambient = scene->getAmbientCoefficient() * scene->getAmbientLightIntensity();
		Eigen::Vector3d color = ambient;

		for (Light& light : lights)
		{
			// Unit vector from the impact point to the lights
			Eigen::Vector3d lightVector = (light.getPosition() - hitPos).normalized();

			// Specular shading depends on the viewing angle of the user (camera). We need to compute the bisector, from the point of view of the impact position,
			// of the ray to the camera and the vector to the lights. The ray's direction vector points from the camera to the impact position, so negative
			// of that points from the impact position to the camera
			Eigen::Vector3d bisector = (-ray.getDirection() + lightVector).normalized();

			// See slide set 5: Final Shading Equation for more on this topic
			// We should note that objectNormal and lightVector are unit vectors, so the dot of them is <= 1


			Eigen::Vector3d diffuse = primitive.getDiffuseCoefficient() * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));
			Eigen::Vector3d specular = primitive.getSpecularCoefficient() * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getPhongExponent()));

			// If shadow ray hits an object, we won't have lights hitting it, so ambient only
			Ray shadow(hitPos, light.getPosition());
			// Ignore self intersection because diffuse/specular dot products will handle it more accurately
			std::shared_ptr<Primitive> shadowHitPtr = scene->getFirstIntersection(shadow, primitivePtr); // TODO: This can *probably* go.
			if (shadowHitPtr != nullptr)
			{
				diffuse = Eigen::Vector3d::Zero();
				specular = Eigen::Vector3d::Zero();
			}

			color += diffuse + specular;
		}

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

	double halfAlpha = std::atan2(1, focalLength);

	double pixelCameraX = screenX * aspectRatio * std::tan(halfAlpha);
	double pixelCameraY = screenY * std::tan(halfAlpha);


	return Eigen::Vector3d(pixelCameraX, pixelCameraY, -1); // We're fixing z = -1
}

unsigned int Camera::getPixelCount()
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

