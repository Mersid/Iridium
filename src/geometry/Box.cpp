#include "Box.h"

#include <utility>
#include <math.h>

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

Box Box::aggregate(const std::vector<Box>& boxes)
{
	double dbMax = std::numeric_limits<double>::max();
	double dbMin = std::numeric_limits<double>::lowest();
	Eigen::Vector3d min(dbMax, dbMax, dbMax);
	Eigen::Vector3d max(dbMin, dbMin, dbMin);

	for (const Box& box : boxes)
	{
		min[0] = std::min(min[0], box.min.x());
		min[1] = std::min(min[1], box.min.y());
		min[2] = std::min(min[2], box.min.z());

		max[0] = std::max(max[0], box.max.x());
		max[1] = std::max(max[1], box.max.y());
		max[2] = std::max(max[2], box.max.z());
	}

	return Box(min, max);
}

bool Box::intersect(const Ray& ray)
{
	// Implementation of the Cyrus-Beck clipping algorithm. See sources for sources

	double t1 = (min.x() - ray.getPosition().x()) / ray.getDirection().x();
	double t2 = (max.x() - ray.getPosition().x()) / ray.getDirection().x();
	double t3 = (min.y() - ray.getPosition().y()) / ray.getDirection().y();
	double t4 = (max.y() - ray.getPosition().y()) / ray.getDirection().y();
	double t5 = (min.z() - ray.getPosition().z()) / ray.getDirection().z();
	double t6 = (max.z() - ray.getPosition().z()) / ray.getDirection().z();

	double tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	double tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// If tMax < 0, it's behind us. If tMax < tMin, we missed.
	return tMax >= 0 && tMax >= tMin;
}
