#include <iostream>
#include "Camera.h"
#include "../texture/Texture.h"
#include "../Shimmerlight.h"
#include "Scene.h" // We can use "Scene.h" here because .cpp files aren't #included, so we won't have to deal with circular dependency

Camera::Camera(int width, int height) :
	width(width), height(height)
{
	// Create the required number of pixel rays
	pixelRays = std::vector<Eigen::Vector3d>(width * height, Eigen::Vector3d(0, 0, 0));

	// We start with an empty "image" represented by a vector of "pixel" rays, each pixel of which corresponds to a single pixel on
	// a texture. However, our camera's pixel rays are from -1 to 1, so we need to scale accordingly.
	// We're converting from raster space, to NDC, to screen space for each pixel to evenly distribute our pixel rays. We can still take
	// advantage of their being in a vector to obtain the original raster coordinates.

	for (std::vector<Eigen::Vector3d>::size_type i = 0; i < pixelRays.size(); i++)
	{
		unsigned int pixelX = i % width;
		unsigned int pixelY = i / width;

		// NDC, between 0 and 1
		double ndcX = (pixelX + 0.5) / width;
		double ndcY = (pixelY + 0.5) / height;

		// Screen space, which our camera expects. Between -1 and 1 for each axis
		double screenX = 2 * ndcX - 1;
		double screenY = 1 - 2 * ndcY;

		Eigen::Vector3d& pixelRay = pixelRays[i];
		pixelRay[0] = screenX;
		pixelRay[1] = screenY;
		pixelRay[2] = -1; // We're fixing z = -1
	}
}

Camera::Camera()
{

}

Texture Camera::takeSnapshot(CameraMode cameraMode)
{
	Texture t(width, height);

	for (std::vector<Eigen::Vector3d>::size_type i = 0; i < pixelRays.size(); i++)
	{
		Eigen::Vector3d& pixelRay = pixelRays[i];
		unsigned int pixelX = i % width;
		unsigned int pixelY = i / width;

		Ray ray;
		if (cameraMode == CameraMode::PERSPECTIVE)
			ray = Ray(Eigen::Vector3d(0, 0, 0), pixelRay);
		else // Orthographic, so fire rays in a straight line
			// RECALL THAT RAY PARAM TAKES POS AND DIR, NOT START AND END. DON'T BE LIKE ME AND MAKE THIS MISTAKE :'(  - Steven, 2022-02-11
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
		hitPos = hitPos + objectNormal * 0.001;

		Light& light = scene->getLight();

		// Unit vector from the impact point to the light
		Eigen::Vector3d lightVector = (light.getPosition() - hitPos).normalized();

		// Specular shading depends on the viewing angle of the user (camera). We need to compute the bisector, from the point of view of the impact position,
		// of the ray to the camera and the vector to the light. The ray's direction vector points from the camera to the impact position, so negative
		// of that points from the impact position to the camera
		Eigen::Vector3d bisector = (-ray.getDirection() + lightVector).normalized();

		// See slide set 5: Final Shading Equation for more on this topic
		// We should note that objectNormal and lightVector are unit vectors, so the dot of them is <= 1
		// Compute ambient color

		// Compute diffuse intensities for each of r, g, b.
		double ambientR = scene->getAmbientCoefficient()[0] * scene->getAmbientLightIntensity();
		double ambientG = scene->getAmbientCoefficient()[1] * scene->getAmbientLightIntensity();
		double ambientB = scene->getAmbientCoefficient()[2] * scene->getAmbientLightIntensity();

		double diffuseR = primitive.getDiffuseCoefficient()[0] * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));
		double diffuseG = primitive.getDiffuseCoefficient()[1] * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));
		double diffuseB = primitive.getDiffuseCoefficient()[2] * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));

		// Compute specular intensities for each of r, g, b
		double specularR = primitive.getSpecularCoefficient()[0] * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getPhongExponent()));
		double specularG = primitive.getSpecularCoefficient()[1] * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getPhongExponent()));
		double specularB = primitive.getSpecularCoefficient()[2] * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getPhongExponent()));

		// If shadow ray hits an object, we won't have light hitting it, so ambient only
		Ray shadow(hitPos, lightVector);
		// Ignore self intersection because diffuse/specular dot products will handle it more accurately
		std::shared_ptr<Primitive> shadowHitPtr = scene->getFirstIntersection(shadow, primitivePtr);
		if (shadowHitPtr != nullptr)
		{
			diffuseR = 0;
			diffuseG = 0;
			diffuseB = 0;

			specularR = 0;
			specularG = 0;
			specularB = 0;
		}

		unsigned char colorR = (unsigned char)(255 * std::clamp(ambientR + diffuseR + specularR, 0.0, 1.0));
		unsigned char colorG = (unsigned char)(255 * std::clamp(ambientG + diffuseG + specularG, 0.0, 1.0));
		unsigned char colorB = (unsigned char)(255 * std::clamp(ambientB + diffuseB + specularB, 0.0, 1.0));

		t.setPixel((signed int)pixelX, (signed int)pixelY, colorR, colorG, colorB);
	}
	return t;
}

void Camera::setScene(Scene &s)
{
	this->scene = &s;
}

