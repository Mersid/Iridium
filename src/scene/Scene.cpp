#include "Scene.h"
#include "../geometry/Sphere.h"

Scene::Scene() : ambientCoefficient(Eigen::Vector3d(1, 1, 1)), ambientLightIntensity(0.005)
{
}

Texture Scene::takeSnapshot(CameraMode cameraMode)
{
	return camera.takeSnapshot(cameraMode);
}

std::vector<Light>& Scene::getLights()
{
	return lights;
}

const Eigen::Vector3d &Scene::getAmbientCoefficient() const
{
	return ambientCoefficient;
}

double Scene::getAmbientLightIntensity() const
{
	return ambientLightIntensity;
}

void Scene::setCamera(Camera c)
{
	camera = c;
	camera.setScene(*this);
}

void Scene::addLight(Light l)
{
	lights.emplace_back(l);
}

void Scene::addPrimitive(std::shared_ptr<Primitive> p)
{
	primitives.emplace_back(p);
}

std::shared_ptr<Primitive> Scene::getFirstIntersection(const Ray& ray, std::shared_ptr<Primitive> ignore)
{
	std::shared_ptr<Primitive> nearestObject = nullptr;
	Eigen::Vector3d nearestHitPos; // The hit pos of the nearest object. We need this to compare with the current object, and replace it if it's closer than this one

	for (const std::shared_ptr<Primitive>& primitive : primitives)
	{
		// Primitive was specified to ignore
		if (primitive == ignore)
			continue;

		std::optional<Eigen::Vector3d> possibleHit = primitive->getRayIntersection(ray);
		if (!possibleHit.has_value())
			continue; // We missed. Continue on with the next object

		// We have a value for possibleHit, so it did hit the primitive we're checking against.
		Eigen::Vector3d& hitPos = possibleHit.value();

		// We hit something, and we haven't hit anything else yet for this pixel, so set it
		// OR: If our new object has a shorter ray than the previous closest object, replace it.
		if (nearestObject == nullptr || (hitPos - ray.getPosition()).norm() < (nearestHitPos - ray.getPosition()).norm())
		{
			nearestObject = primitive;
			nearestHitPos = hitPos;
		}
	}

	return nearestObject;
}
