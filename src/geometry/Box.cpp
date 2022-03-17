#include "Box.h"

#include <utility>

Box::Box(Eigen::Vector3d min, Eigen::Vector3d max) : min(std::move(min)), max(std::move(max))
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
