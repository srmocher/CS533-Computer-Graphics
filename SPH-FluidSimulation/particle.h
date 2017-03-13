#pragma once
#include <glm/glm.hpp>

class Particle
{
public:
	float mass;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float density;
	glm::vec3 viscosity;
	glm::vec3 color_field;
	glm::vec3 color_gradient;
	float pressure_gradient;
	glm::vec3 viscosity_force;
	glm::vec3 surface_force;
	float pressure;
	float gas_constant;
	
};