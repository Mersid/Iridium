#pragma once

#include "Eigen/Core"
#include "yaml-cpp/yaml.h"

template<>
struct YAML::convert<Eigen::Vector3d>
{
	static bool decode(const Node& node, Eigen::Vector3d& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs[0] = node[0].as<double>();
		rhs[1] = node[1].as<double>();
		rhs[2] = node[2].as<double>();
		return true;
	}
};