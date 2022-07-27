#pragma once

#include "../misc/Transform.h"
#include "MeshDef.h"

// TODO
struct ModelDef
{
	bool useBvh;
	Transform transform;
	MeshDef meshDef;
};

template<>
struct YAML::convert<ModelDef>
{
	static bool decode(const Node& node, ModelDef& def)
	{

	}
};
