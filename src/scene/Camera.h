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

	/**
	 * Gets the pixel ray's position on the virtual plane of the camera. This allows us to lazily compute the pixel ray position.
	 * Testing shows that a 1600 x 1600 image can use something like 420.9 MB of RAM, most of which is just the precomputed
	 * pixel ray positions.
	 * NOTE: This technique reduced memory usage from ~420.9 MB for the multi2 image down to ~69.3 MB with what appears to be negligible difference in speed:
	 * 		 The first run took 196 seconds the second took 203 seconds.
	 * @param i The pixel value to return the position for. In 2d, the value i is y * width + x, starting with the first pixel at i = 0.
	 * 			Ensure that the value is less than the number of pixels, or it will fall off the bottom edge.
	 * @return The pixel ray position along the virtual plane.
	 */
	Eigen::Vector3d getPixelRayAt(int i);

	/**
	 * @return The number of pixels that comprise the camera's virtual plane.
	 */
	unsigned int pixelCount();

private:
	int width;
	int height;

	Scene* scene; // Which scene the camera is a part of. Can be null if it's not in a scene. This is needed to access other objects in the scene.
};


