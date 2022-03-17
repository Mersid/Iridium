#include "Box.h"

Box::Box(Eigen::Vector3d min, Eigen::Vector3d max) : min(min), max(max)
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


