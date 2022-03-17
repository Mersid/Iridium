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
	explicit Model(const Mesh& mesh, const Material& material = Material());

	/**
	 * @param ray The ray to fire against the model
	 * @return A RayTraceInfo with information about the hit primitive, if it hit.
	 */
	std::optional<RayTraceInfo> intersect(const Ray& ray);

	void setMaterial(const Material& material);

	/**
	 * @param translateBy Translates the model by this amount
	 */
	void translate(Eigen::Vector3d translateBy);

	Box& getBoundingBox();


private:
	Mesh mesh;
	Box boundingBox;
	BoundingVolumeHierarchy bvh;

	void computeBoundingBox();
};


