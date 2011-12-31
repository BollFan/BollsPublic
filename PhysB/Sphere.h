#pragma once
#include <glm\glm.hpp>


float randUnit();

void drawCylinder(float* p, float rb, float rt , float rh, float startAngle=0.0f);

void drawLineFromTo( glm::vec3 from , glm::vec3 to );
void drawLineCenterVector( glm::vec3 center , glm::vec3 v);

class Sphere{
public:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 acc;
	glm::vec3 col;
	float r;
	float m;
	Sphere( float r = 1.0f, float m = 1.0f, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 vel = glm::vec3(0.0f), glm::vec3 acc = glm::vec3(0.0f), glm::vec3 col = glm::vec3( randUnit(), randUnit(), randUnit() ) );
	void draw() const;
};