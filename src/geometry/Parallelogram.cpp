#include "Parallelogram.h"

Parallelogram::Parallelogram(Eigen::Vector3d a, Eigen::Vector3d b, Eigen::Vector3d c, Material material) :
	a(a), b(b), c(c), u(b - a), v(c - a), normal(u.cross(v)),
	material(material)
{
}

std::optional<Eigen::Vector3d> Parallelogram::getRayIntersection(const Ray& ray)
{
	// See https://www.desmos.com/calculator/tzhhydtblv for more info. We have all variables needed already at this point
	// For debugging, a=[-1, -1, 0], b = [-4, -2, 2], c = [-4, 1, -1], ray.position = [1, 2, 6], ray.direction = [-1.6, -0.4, -1] should hit the vertex implied in the parallelogram with s, t = 1.

	// Plane and ray are parallel, and will never intersect. Trying to compute it will cause div/0.
	if (ray.getDirection().dot(normal) == 0)
		return std::nullopt;

	// The value of t for the ray's intersection, given by ray.position + t * ray.direction;
	double rayParameterization = (a - ray.getPosition()).dot(normal) / ray.getDirection().dot(normal);

	// If t <= 0, the parallelogram is behind the ray or inside it, which we will not consider an impact.
	if (rayParameterization <= 0)
		return std::nullopt;

	// The impact position of the ray in 3d space. Should be identical to the one returned by the plane intercept algorithm,
	// with perhaps slight differences due to floating-point errors. This is used to discover the parameterization values s and t
	// for the parallelogram (the multipliers along the u and v axes, respectively, to parameterize the parallelogram).
	Eigen::Vector3d rayImpact = ray.getPosition() + rayParameterization * ray.getDirection();

	//https://web.archive.org/web/20110716101940/http://www.softsurfer.com/Archive/algorithm_0105/algorithm_0105.htm.
	// w is the vector from "a" to the impact rayImpact.
	Eigen::Vector3d w = rayImpact - a;

	double s = (u.dot(v) * w.dot(v) - v.dot(v) * w.dot(u)) / (u.dot(v) * u.dot(v) - u.dot(u) * v.dot(v));
	double t = (u.dot(v) * w.dot(u) - u.dot(u) * w.dot(v)) / (u.dot(v) * u.dot(v) - u.dot(u) * v.dot(v));

	// If s and t are both between 0 and 1, then the point computed by rayImpact is within the parallelogram.
	// If s or t is less than 0 or greater than 1, it fell outside in one way or another.
	// We can use s and t and parameterize by doing a + su + tv, but since it should be identical to rayImpact, we'll save ourselves the trouble.
	if (s <= 1 && t <= 1 && s >= 0 && t >= 0)
		return rayImpact;

	return std::nullopt;
}


Eigen::Vector3d Parallelogram::getNormalAt(Eigen::Vector3d position)
{
	return (b - a).cross(c - a).normalized();
}

Material& Parallelogram::getMaterial()
{
	return material;
}

void Parallelogram::setMaterial(Material material)
{
	this->material = material;
}

void Parallelogram::move(Eigen::Vector3d newPos)
{
	a += newPos;
	b += newPos;
	c += newPos;
}

