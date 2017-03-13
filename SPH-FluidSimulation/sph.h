#pragma once
#include "particle.h"


#include <vector>

class SPH
{
private:
	std::vector<Particle> particles;
public:
	void init(int);
	void calc_density();
	void calc_pressure_force();
	void calc_viscosity_force();
	void calc_force_surface_tension();
	void calc_position_velocity();
	void draw();

};