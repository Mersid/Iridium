#include "Material.h"

Material::Material(Eigen::Vector3d diffuseCoefficient, Eigen::Vector3d specularCoefficient, double phongExponent,
                   double reflectionCoefficient, std::optional<Texture> texture) :
				   diffuseCoefficient(diffuseCoefficient), specularCoefficient(specularCoefficient), phongExponent(phongExponent), reflectionCoefficient(reflectionCoefficient), texture(texture)
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

const std::optional<Texture>& Material::getTexture() const
{
	return texture;
}
