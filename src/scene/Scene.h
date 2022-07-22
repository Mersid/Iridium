#pragma once

#include "Eigen/Core"
#include "Camera.h"
#include "../geometry/Model.h"
#include <memory>

class Scene
{
public:
	Scene();

	std::vector<Light>& getLights();

	/**
	 * Sets the given camera as the camera. This camera is copied
	 * @param c The camera to pass to the scene
	 */
	void setCamera(Camera c);

	/**
	 * Adds a light to the scene
	 * @param l The light to add to the scene
	 */
	void addLight(const Light& l);

	/**
	 * @param model The model to add to the scene
	 */
	void addModel(const Model& model);

	/**
	 * Takes a snapshot with the camera in the scene
	 * @param cameraMode Take the snapshot in perspective or orthographic mode
	 * @return The texture containing what this camera captured when the method was called
	 */
	Texture render(CameraMode cameraMode = CameraMode::PERSPECTIVE);

	/**
	 * Finds the first object the ray intersects.
	 * @param ray The ray to cast
	 * @return A pointer to the object that was hit first, or a pointer to nullptr if it missed
	 */
	Primitive* getFirstIntersection(const Ray& ray);

	[[nodiscard]] const Eigen::Vector3d& getAmbientCoefficient() const;

	[[nodiscard]] double getAmbientLightIntensity() const;

	/**
	 * Takes in a ray from a position with direction, and tests if it hits an object in the scene. This method is recursive,
	 * and can trace its own rays to compute reflection and refraction effects.
	 * @param ray A ray cast from a point in space, in a direction. If this ray hits an object, it will (recursively) compute the color
	 * of the hit object
	 * @param ttl The time-to-live for the ray. If the method is called with 0, the method will return nothing
	 * @return Vector representing the color traced from the ray. If the ray missed or TTL is 0, returns nothing
	 */
	std::optional<Eigen::Vector3d> trace(const Ray& ray, int ttl = 1);

private:
	Camera camera;
	std::vector<Model> models;
	std::vector<Light> lights;

	Eigen::Vector3d ambientCoefficient;
	double ambientLightIntensity{};

};


