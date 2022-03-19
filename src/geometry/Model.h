#pragma once


#include "Mesh.h"
#include "Box.h"
#include "BoundingVolumeHierarchy.h"
#include "RayTraceInfo.h"

/**
 * A model consists of a mesh and associated information, such as the bounding box and the BVH
 */
class Model
{
public:
	/**
	 * @param mesh Constructs a model from the given mesh
	 * @param material The material to be applied to each of the triangles
	 */
	explicit Model(Mesh mesh, const Material& material = Material());

	/**
	 * @param ray The ray to fire against the model
	 * @return A RayTraceInfo with information about the hit primitive, if it hit.
	 */
	std::optional<RayTraceInfo> intersect(const Ray& ray);

	void setMaterial(const Material& material);

	/**
	 * @param translateBy Translates the model by this amount
	 */
	void translate(const Eigen::Vector3d& translateBy);

	/**
	 * Generates a bounding volume hierarchy for this model
	 */
	void generateBVH();


private:
	Mesh mesh;
	std::shared_ptr<BoundingVolumeHierarchy> bvh;

	/**
	 * Gets a short-listed list of possible intersects. Since this list is likely *much* shorter than the list of all primitives,
	 * it is much faster. They are possible intersects because it only computes ray intersections with the bounding boxes that
	 * contain the primitive. The ray can go through objects.
	 * @param ray The ray to test collisions for
	 * @return A list possible hits. More collisions will have to be considered to ensure a hit and also which one is closest.
	 */
	std::vector<Primitive*> getPossibleIntersects(const Ray& ray);
};


