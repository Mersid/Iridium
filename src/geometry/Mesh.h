#pragma once

#include "Primitive.h"
#include "yaml-cpp/yaml.h"
#include "Parallelogram.h"

/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	// TODO: Maybe unique_ptr?
	void addPrimitive(std::shared_ptr<Primitive>&& primitive);

	std::vector<Primitive*> getPrimitives();

	Material& getMaterial();

	void setMaterial(const Material& material);

private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	Material material;
};

template<>
struct YAML::convert<Mesh>
{
	static bool decode(const Node& node, Mesh& mesh)
	{

		if (node["parallelogram"])
		{
			//auto parallelogram = node["parallelogram"].as<Parallelogram>();
			//parallelogram.setMesh(&mesh);
			//mesh.addPrimitive(std::make_shared<Parallelogram>(std::move(parallelogram)));
			
		}
		else if (node["sphere"])
		{

		}
		else if (node["triangle"])
		{

		}
		else
		{

		}
	}
};
