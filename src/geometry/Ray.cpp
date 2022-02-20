#include "Ray.h"

const Eigen::Vector3d& Ray::getPosition() const
{
	return position;
}

const Eigen::Vector3d& Ray::getDirection() const
{
	return direction;
}

Ray::Ray(Eigen::Vector3d position, Eigen::Vector3d pos2) :
	position(position), direction((pos2 - position).normalized())
{

}

Ray::Ray() :
	position(Eigen::Vector3d(0, 0, 0)), direction(Eigen::Vector3d(0, 0, 0))
{

}
