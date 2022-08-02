#include "Light.h"
#include "../misc/Vector3dConvert.h"
#include <utility>

Light::Light(Eigen::Vector3d position, double intensity) :
	position(std::move(position)), intensity(intensity)
{

}

const Eigen::Vector3d& Light::getPosition() const
{
	return position;
}

double Light::getIntensity() const
{
	return intensity;
}

Light Light::deserialize(const YAML::Node& node)
{
	auto position = node["position"].as<Eigen::Vector3d>();
	auto intensity = node["intensity"].as<double>();

	Light light(position, intensity);
	return light;
}

