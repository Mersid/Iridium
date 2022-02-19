#pragma once


#include "Eigen/Core"
#include "../geometry/Parallelogram.h"
#include "CameraMode.h"
#include "../texture/Texture.h"
#include "Light.h"

class Scene; // Forward declaration because #include "Scene.h" will cause circular dependency

class Camera
{
public:
	/**
	 * Creates a camera with a given number of pixels. A ray will be fired for each pixel and a Texture generated thereof.
	 * The camera essentially consists of the pinhole coordinates (from where all perspective rays are fired), and a plane of vectors
	 * representing the virtual film. One vector will represent each dot, which in turn represents one pixel.
	 * The camera is at the origin, and the raster plane is at z = -1, and spans x and y from -1 to 1 (occupying 2 x 2 units).
	 * The pixels are distributed evenly along each axis.
	 * If used in orthographic mode, the initial coordinates of the ray will be equal to the pixel ray, except at z = 0 instead of z = -1.
	 * @param width The width of the camera, in pixels
	 * @param height The height of the camera, in pixels
	 */
	Camera(int width, int height);

	/**
	 * Default constructor for object initialization. Do not attempt to use, behaviour is undefined and may crash or segfault.
	 */
	Camera();

	/**
	 * Takes a snapshot of what the camera sees
	 * @param cameraMode Take the snapshot in orthographic or perspective mode
	 * @return A texture depicting what the camera saw at each of its pixel rays.
	 */
	Texture takeSnapshot(CameraMode cameraMode = CameraMode::PERSPECTIVE);

	/**
	 * Sets the scene of this camera. This is needed for the camera to access other objects in the scene
	 * @param s The scene to put this camera in
	 */
	void setScene(Scene& s);

private:
	int width;
	int height;
	std::vector<Eigen::Vector3d> pixelRays;

	Scene* scene; // Which scene the camera is a part of. Can be null if it's not in a scene. This is needed to access other objects in the scene.
};


