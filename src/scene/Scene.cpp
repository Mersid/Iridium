#include "Scene.h"

Scene::Scene() : ambientCoefficient(Eigen::Vector3d(1, 1, 1)), ambientLightIntensity(0.005)
{
}

Texture Scene::render(CameraMode cameraMode)
{
	// TODO / BUG: Pay off technical debt. Beyond this being a hack-job, we have to generate the models' BVHs
	// TODO: in-place because otherwise when copying models with a BVH the BVH is copied... but it points to the old
	// TODO: model's primitives, so if that old copy disappears or the new one is updated or whatever, it will not reflect
	// TODO: the actual situation, and we will likely end up with a crash for dereferencing invalid memory.
	for (Model& m : models)
		m.generateBVH();

	Texture t = camera.takeSnapshot(cameraMode, 1);

	models.clear();
	return t;
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

void Scene::addLight(const Light& l)
{
	lights.emplace_back(l);
}

void Scene::addModel(const Model& model)
{
	models.emplace_back(model);
}

Primitive* Scene::getFirstIntersection(const Ray& ray)
{
	Primitive* nearestObject = nullptr;
	Eigen::Vector3d nearestHitPos; // The hit pos of the nearest object. We need this to compare with the current object, and replace it if it's closer than this one

	for (Model& model : models)
	{
		std::optional<RayTraceInfo> possibleHit = model.intersect(ray);
		if (!possibleHit.has_value())
			continue; // We missed. Continue on with the next object

		// We have a value for possibleHit, so it did hit the primitive we're checking against.
		RayTraceInfo rayTraceInfo = *possibleHit;

		// We hit something, and we haven't hit anything else yet for this scan, so set it
		// OR: If our new object has a shorter ray than the previous closest object, replace it.
		if (nearestObject == nullptr || (rayTraceInfo.getHitPos() - ray.getPosition()).norm() < (nearestHitPos - ray.getPosition()).norm())
		{
			nearestObject = rayTraceInfo.getHitPrimitive();
			nearestHitPos = rayTraceInfo.getHitPos();
		}
	}

	return nearestObject;
}

std::optional<Eigen::Vector3d> Scene::trace(const Ray& ray, int ttl)
{
	// Find the nearest primitive we'll hit
	Primitive* primitivePtr = getFirstIntersection(ray);

	// We hit nothing, then skip this pixel
	if (primitivePtr == nullptr || ttl <= 0)
		return std::nullopt;

	Primitive& primitive = *primitivePtr;
	std::optional<Eigen::Vector3d> optHitPos = primitive.getRayIntersection(ray);
	if (!optHitPos.has_value()) // Not needed anymore because we refactored above to guarantee primitive hit, but we'll leave it for now
		return std::nullopt;

	// Compute ray colors
	Eigen::Vector3d hitPos = optHitPos.value();
	Eigen::Vector3d objectNormal = primitive.getNormalAt(hitPos);

	// Handle case of backwards-facing plane. The incoming ray and the normal should face opposite directions.
	// In this case, they aren't, so fix it by changing the normal of the object
	if (objectNormal.dot(ray.getDirection()) > 0)
		objectNormal = -objectNormal;

	// Move hitPos in the normal direction a bit so that the shadow ray won't hit the object itself.
	hitPos = hitPos + objectNormal * 1e-6;

	Eigen::Vector3d ambient = ambientCoefficient * ambientLightIntensity;
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

		Eigen::Vector3d diffuse = primitive.getMesh()->getMaterial().getDiffuseCoefficient() * light.getIntensity() * std::max(0.0, objectNormal.dot(lightVector));
		Eigen::Vector3d specular = primitive.getMesh()->getMaterial().getSpecularCoefficient() * light.getIntensity() * (std::pow(std::max(0.0, objectNormal.dot(bisector)), primitive.getMesh()->getMaterial().getPhongExponent()));

		// If shadow ray hits an object, we won't have lights hitting it, so ambient only
		Ray shadow(hitPos, light.getPosition());

		Primitive* shadowHitPtr = getFirstIntersection(shadow);

		// If we hit an object, get that object and test again (it's inefficient, I know): if the hit pos of the object between the first object and the light
		// is less than the distance to the light, then the object is between the light and the first object, and is therefore in shadow.
		if (shadowHitPtr != nullptr && (shadowHitPtr->getRayIntersection(shadow).value() - hitPos).norm() < (light.getPosition() - hitPos).norm())
		{
			diffuse = Eigen::Vector3d::Zero();
			specular = Eigen::Vector3d::Zero();
		}

		color += diffuse + specular;
	}

	Eigen::Vector3d reflectDirection = ray.getDirection() - 2 * ray.getDirection().dot(objectNormal) * objectNormal;
	Ray reflectRay(hitPos, hitPos + reflectDirection);

	std::optional<Eigen::Vector3d> optReflectColor = trace(reflectRay, ttl - 1);
	color += primitive.getMesh()->getMaterial().getReflectionCoefficient() * (optReflectColor.has_value() ? optReflectColor.value() : Eigen::Vector3d::Zero());

	return color;
}

Scene Scene::deserialize(const YAML::Node& node)
{
	Scene scene;
	Camera camera = Camera::deserialize(node["camera"]);
	scene.setCamera(camera);

	for (const YAML::Node& node : node["lights"])
		scene.addLight(Light::deserialize(node));

	for (const YAML::Node& node : node["models"])
		scene.addModel(Model::deserialize(node));

	return scene;
}


