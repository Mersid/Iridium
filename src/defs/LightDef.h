#pragma once

#include "Eigen/Core"
#include "yaml-cpp/yaml.h"
#include "../misc/Vector3dConvert.h"

struct LightDef
{
	Eigen::Vector3d position;
	double intensity;
};

template<>
struct YAML::convert<LightDef>
{
	static bool decode(const Node& node, LightDef& def)
	{
		def.position = node["position"].as<Eigen::Vector3d>();
		def.intensity = node["intensity"].as<double>();
	}
};
