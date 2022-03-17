#include "Light.h"

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

