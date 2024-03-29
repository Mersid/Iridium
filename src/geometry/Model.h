#pragma once


#include "Mesh.h"
#include "Box.h"
#include "BoundingVolumeHierarchy.h"
#include "RayTraceInfo.h"
#include "../misc/Transform.h"

/**
 * A model consists of a mesh and associated information, such as the bounding box and the BVH
 */
class Model
{
public:

	/**
	 * Instantiates a new, empty model that can be manipulated to add things like models This is exposed
	 * because models should not be stack allocated, as child objects like meshes may point to this and thus should not be moved
	 */
	static std::unique_ptr<Model> instantiate(std::unique_ptr<Mesh> mesh);

	void setMesh(std::unique_ptr<Mesh> mesh);

	/**
	 * @param ray The ray to fire against the model
	 * @return A RayTraceInfo with information about the hit primitive, if it hit.
	 */
	std::optional<RayTraceInfo> intersect(const Ray& ray);

	/**
	 * Generates a bounding volume hierarchy for this model
	 */
	void generateBVH();

    /**
     * TODO: If we apply it twice it will be applied twice. We should probably check if it's already applied
     */
    void applyTransform();

	Transform& getTransform();

	static std::unique_ptr<Model> deserialize(const YAML::Node& node);


private:
	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<BoundingVolumeHierarchy> bvh;

	Transform transform;

	/**
	 * @param mesh Constructs a model from the given mesh
	 */
	explicit Model(std::unique_ptr<Mesh> mesh);

	/**
	 * Gets a short-listed list of possible intersects. Since this list is likely *much* shorter than the list of all primitives,
	 * it is much faster. They are possible intersects because it only computes ray intersections with the bounding boxes that
	 * contain the primitive. The ray can go through objects.
	 * @param ray The ray to test collisions for
	 * @return A list possible hits. More collisions will have to be considered to ensure a hit and also which one is closest.
	 */
	std::vector<Primitive*> getPossibleIntersects(const Ray& ray);
};
