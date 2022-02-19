#include <iostream>
#include "Light.h"

Light::Light(Eigen::Vector3d position, double intensity) :
	position(position), intensity(intensity)
{

}

const Eigen::Vector3d &Light::getPosition() const
{
	return position;
}

double Light::getIntensity() const
{
	return intensity;
}

Light::Light()
{

}
