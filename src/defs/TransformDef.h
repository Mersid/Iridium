#pragma once

#include "Eigen/Core"
#include "yaml-cpp/yaml.h"
#include "../misc/Vector3dConvert.h"

struct TransformDef
{
	Eigen::Vector3d position;
	Eigen::Vector3d rotation;
	Eigen::Vector3d scale;
};

template<>
struct YAML::convert<TransformDef>
{
	static bool decode(const Node& node, TransformDef& def)
	{
		def.position = node["position"].as<Eigen::Vector3d>();
		def.rotation = node["rotation"].as<Eigen::Vector3d>();
		def.scale = node["scale"].as<Eigen::Vector3d>();
	}
};
