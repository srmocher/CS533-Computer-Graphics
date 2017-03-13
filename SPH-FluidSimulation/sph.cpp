#include "sph.h"
#include <vector>
#include <glm/glm.hpp>
#include <CompactNSearch.h>
#include <glm/gtx/norm.hpp>
float REST_DENSITY = 1000;
float H = 0.03;
float PI = 3.14;
float SIGMA = 0.001;

void SPH::init(int numParticles)
{
	float scale = 0.004;
	glm::vec3 pos(0.0,0.0,0.0);
	for (int i = 0;i < numParticles;i++)
	{
		Particle p;
		p.mass = 0.012;
		p.gas_constant = 461;
		p.position = pos;
		pos.x += scale;
		pos.y += scale;
		pos.z += scale;
		p.velocity = glm::vec3(0.0, 0.0, 0.0);
		p.viscosity = glm::vec3(0.05, 0.05, 0.05);
		particles.push_back(p);
	}
	
}

void SPH::calc_density()
{
	for (int i = 0;i < particles.size();i++)
	{
		Particle p = particles[i];
		float density = 0.0f;
		for (int j = 0;j < particles.size();j++)
		{
			Particle q = particles[j];
			float r_square = glm::distance2(p.position, q.position);
			if(r_square <= pow(H,2))
				density += particles[j].mass*(315 / (64 * PI*pow(H, 9)))*(pow((pow(H, 2) - r_square),3));
		}
		p.density = density;
	}
}

void SPH::calc_pressure_force()
{
	for (int i = 0;i < particles.size();i++)
	{
		particles[i].pressure = particles[i].gas_constant*(particles[i].density - REST_DENSITY);
	}

	for (int i = 0;i < particles.size();i++)
	{
		Particle p = particles[i];
		float pressure_force = 0.0;
		for (int j = 0;j < particles.size();j++)
		{
			Particle q = particles[j];
			float r_square = glm::distance2(p.position, q.position);
			float r = sqrt(r_square);
			if (r <= H)
			{
				pressure_force += q.mass *((p.pressure + q.pressure) / 2 * q.pressure)*(-45 / (PI*pow(H, 6)))*pow(H - r, 2);
			}
		}
		p.pressure_gradient = - pressure_force;
	}
}

void SPH::calc_viscosity_force()
{
	for (int i = 0;i < particles.size();i++)
	{
		Particle p = particles[i];
		glm::vec3 viscosity_force = glm::vec3(0.0,0.0,0.0);
		for (int j = 0;j < particles.size();j++)
		{
			Particle q = particles[j];
			float r_square = glm::distance2(p.position, q.position);
			float r = sqrt(r_square);
			if (r_square <= pow(H, 2))
			{
				auto vel_diff = (q.velocity - p.velocity)/q.density;
				float kernel_val = 45.0f / (PI*pow(H, 6))*(H - r);
				viscosity_force += q.viscosity * q.mass * vel_diff * kernel_val;
			}
		}
		p.viscosity_force = viscosity_force;
	}
}

void SPH::calc_force_surface_tension()
{
	for (int i = 0;i < particles.size();i++)
	{
		Particle p = particles[i];
		glm::vec3 color_field(0.0, 0.0, 0.0),color_gradient(0.0,0.0,0.0),color_laplacian(0.0,0.0,0.0);
		for(int j = 0;j < particles.size();j++)
		{
			Particle q = particles[j];
			float r_square = glm::distance2(p.position, q.position);
			float r = sqrt(r_square);
			if (r_square <= pow(H, 2))
			{
				color_field += (q.mass / q.density)*(315 / (64 * PI*pow(H, 9)))*(pow((pow(H, 2) - r_square), 3));
				color_gradient += (q.mass / q.density)*(-945 / (32 * PI*pow(H, 9)))*r*pow((pow(H, 2) - pow(r, 2)), 2);
				color_laplacian += (q.mass / q.density)*(-945 / 32 * PI*pow(H, 9))*(pow(H,2)-pow(r,2))*(3*pow(H,2)-7*pow(r,2));
			}			
		}
		p.color_field = color_field;
		p.color_gradient = color_gradient;
		p.surface_force = SIGMA*color_laplacian*color_gradient / glm::length(color_gradient);
	}
}