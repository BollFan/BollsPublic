#pragma once

#include <vector>
#include "Sphere.h"

class PhysB
{
private:
	float gravity;
	float timeDelta;
	float bounceCoefficient;
	float frictionCoefficient;
	float speedLimitLow;
	
	std::vector< Sphere > objects;
	std::vector< Sphere > sleepies;

	bool collide_sphere_sphere( Sphere* a, Sphere* b );

public:

	void init();

	void update();

	void draw();

	void destroy();

};