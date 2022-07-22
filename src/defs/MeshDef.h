#pragma once
#include "yaml-cpp/yaml.h"

// TODO
struct MeshDef
{

};

template<>
struct YAML::convert<MeshDef>
{
	static bool decode(const Node& node, MeshDef& def)
	{

	}
};
