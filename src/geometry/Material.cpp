#include "Material.h"

#include <utility>

Material::Material(Eigen::Vector3d diffuseCoefficient, Eigen::Vector3d specularCoefficient, double phongExponent,
                   double reflectionCoefficient) :
				   diffuseCoefficient(std::move(diffuseCoefficient)), specularCoefficient(std::move(specularCoefficient)), phongExponent(phongExponent), reflectionCoefficient(reflectionCoefficient)
{
}

const Eigen::Vector3d& Material::getDiffuseCoefficient() const
{
	return diffuseCoefficient;
}

const Eigen::Vector3d& Material::getSpecularCoefficient() const
{
	return specularCoefficient;
}

double Material::getPhongExponent() const
{
	return phongExponent;
}

double Material::getReflectionCoefficient() const
{
	return reflectionCoefficient;
}
