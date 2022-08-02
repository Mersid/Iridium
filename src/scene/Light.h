#pragma once

#include "Eigen/Core"
#include "yaml-cpp/yaml.h"

class Light
{
public:
	explicit Light(Eigen::Vector3d position, double intensity = 1);

	/**
	 * Default constructor for object initialization. Do not attempt to use, behaviour is undefined and may crash or segfault.
	 */
	Light() = default;

	[[nodiscard]] const Eigen::Vector3d& getPosition() const;

	[[nodiscard]] double getIntensity() const;

	static Light deserialize(const YAML::Node& node);

private:
	Eigen::Vector3d position;
	double intensity; // Intensity of the light
};


