#pragma once

#include "Eigen/Core"
#include "Camera.h"
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
	void addLight(Light l);

	/**
	 * Adds a new primitive to the scene
	 * @param p The primitive to add to the scene
	 */
	void addPrimitive(std::shared_ptr<Primitive> p);

	/**
	 * Takes a snapshot with the camera in the scene
	 * @param cameraMode Take the snapshot in perspective or orthographic mode
	 * @return The texture containing what this camera captured when the method was called
	 */
	Texture takeSnapshot(CameraMode cameraMode = CameraMode::PERSPECTIVE);

	/**
	 * Finds the first object the ray intersects
	 * @param ray The ray to cast
	 * @param ignore Ignores the specified primitive from the intersection calculation, computing the next one if necessary
	 * @return A shared pointer to the object that was hit first, or a shared pointer to nullptr if it missed
	 */
	std::shared_ptr<Primitive> getFirstIntersection(const Ray& ray, std::shared_ptr<Primitive> ignore = nullptr);

	const Eigen::Vector3d& getAmbientCoefficient() const;

	double getAmbientLightIntensity() const;

private:
	Camera camera;
	std::vector<std::shared_ptr<Primitive>> primitives;
	std::vector<Light> lights;

	Eigen::Vector3d ambientCoefficient;
	double ambientLightIntensity{};

};


