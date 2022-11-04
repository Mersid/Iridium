#pragma once

#include "Primitive.h"
#include "yaml-cpp/yaml.h"
#include "Parallelogram.h"
#include "Sphere.h"
#include "Triangle.h"


/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	// TODO: Maybe unique_ptr?
	void addPrimitive(std::shared_ptr<Primitive> primitive);

	std::vector<Primitive*> getPrimitives();

	Material& getMaterial();

	void setMaterial(const Material& material);

	/**
	 * Sets the pointer of the mesh of each primitive to this. We cannot do this during construction because the mesh is constructed on the stack and then moved to a shared pointer,
	 * so attempting to do so then will cause invalid memory when the mesh is removed on return.
	 */
	void setPrimitivesOwner();

	static std::unique_ptr<Mesh> deserialize(const YAML::Node& node);

private:
	Mesh() = default;

	std::vector<std::shared_ptr<Primitive>> primitives;
	Material material;
};
