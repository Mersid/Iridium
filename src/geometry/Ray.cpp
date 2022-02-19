#include "Ray.h"

const Eigen::Vector3d& Ray::getPosition() const
{
	return position;
}

const Eigen::Vector3d& Ray::getDirection() const
{
	return direction;
}

Ray::Ray(Eigen::Vector3d position, Eigen::Vector3d direction) :
	position(position), direction(direction.normalized())
{

}

Ray::Ray() :
	position(Eigen::Vector3d(0, 0, 0)), direction(Eigen::Vector3d(0, 0, 0))
{

}
