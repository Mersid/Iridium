#include "Box.h"

Box::Box(const Eigen::Vector3d& min, const Eigen::Vector3d& max) : min(min), max(max)
{

}

Eigen::Vector3d& Box::getMin()
{
	return min;
}

Eigen::Vector3d& Box::getMax()
{
	return max;
}
