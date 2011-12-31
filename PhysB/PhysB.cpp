#include <windows.h>		// Header File For Windows
#include <math.h>			// Math Library Header File
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <glm\glm.hpp>

#include <time.h>
const double PI = 3.1415926535897;
using namespace glm;

#include "PhysB.h"

void PhysB::init()
{
	gravity				= -9.81f;
	timeDelta			= 1.0f / 5000.0f;
	bounceCoefficient	= 0.8f;
	frictionCoefficient	= 0.99f;
	speedLimitLow       = 0.1f;

	//srand( size_t(time(0)) );

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for(int i=0; i<300; ++i)
	{
		Sphere s( 0.2f, 1.0f, vec3( -3+6*randUnit(), 10*randUnit(), 3*randUnit() ) );
		objects.push_back(s);
	}
	//objects.push_back( Sphere( 0.2f,  1.0f, vec3(  0.1, 3.0f, 0 ) ));
	//objects.push_back( Sphere( 0.5f,  1.0f, vec3(  0.0, 0.0f, 0 ) ));

	for(size_t a=0; a<objects.size(); ++a)
		objects[a].acc.y = gravity;
	
	//objects[1].acc = vec3(0);
}

bool PhysB::collide_sphere_sphere( Sphere* a, Sphere* b )
{
	// !intersect(aabb(a),aabb(b))
	if( a->pos.x - a->r > b->pos.x + b->r ||
		a->pos.x + a->r < b->pos.x - b->r ||
		a->pos.y - a->r > b->pos.y + b->r ||
		a->pos.y + a->r < b->pos.y - b->r ||
		a->pos.z - a->r > b->pos.z + b->r ||
		a->pos.z + a->r < b->pos.z - b->r )
		return false;
			
	// Distansvektorn, dvs vart är object b gentemot objekt a
	vec3 dir = a->pos - b->pos;
			
	// Distansen i kvadrat (för snabbare testning)
	float distSquare = dot(dir,dir);

	// Så.. om det är en kollision mellan de två kloten a och b
	float minDist = a->r + b->r;
	if( distSquare <= minDist*minDist )
	{
		// Distansen
		float dist = sqrt(distSquare);
		vec3 N = dir / dist;

		// Hur mycket av deras totala massa tillhör a respektive b
		float aMassFactor = (a->m / (a->m + b->m));
		float bMassFactor = (b->m / (a->m + b->m));
				
		// a och b kolliderar ju, vilket gör att de är lite innuti varandra,
		// så vi flyttar isär dom lite
		float intersectLength = minDist - dist + (a->r+b->r)*0.01f;
		a->pos += ( N) * intersectLength * bMassFactor;
		b->pos += (-N) * intersectLength * aMassFactor;

		// normalisera dir så får vi normalen som visar hur de två kloten kolliderade
		// Vilka punkter på kloten var det som nuddade varandra
		vec3 V = a->vel;
		float Vlength = sqrt(dot(V,V));
		V/=Vlength;

		vec3 R = V - 2 * dot(V, N) * N;

		a->vel = a->vel * aMassFactor + ( R) * Vlength * bMassFactor;
		b->vel = b->vel * bMassFactor + (-R) * Vlength * aMassFactor;

		a->pos += a->vel;
		b->pos += b->vel;

		return true;
	}
	return false;
}

void PhysB::update()
{
	// Add more velocity from the acceleration, like the gravity
	for(size_t a=0; a<objects.size(); ++a)
	{
		objects[a].vel += objects[a].acc * vec3(timeDelta);
		objects[a].pos += objects[a].vel;
	}

	// Gå igenom de klot som rör på sig
	for(size_t a=0; a<objects.size(); ++a)
	{
		// World-Floor-collision
		if( objects[a].pos.y - objects[a].r + objects[a].vel.y <= 0 )
		{
			objects[a].pos.y  = objects[a].r;
			objects[a].vel.y *=- bounceCoefficient;
			objects[a].vel   *= frictionCoefficient;
		}

		// Kollidera kloten med de andra rörliga kloten
		for(size_t b=a+1; b<objects.size(); ++b)
			collide_sphere_sphere( &objects[a] , &objects[b] );

		// Kollidera kloten med de klot som sover
		for(size_t b=0; b<sleepies.size(); ++b)
		{
			// Om de kolliderade
			if( collide_sphere_sphere( &objects[a] , &sleepies[b] ) )
			{
				// Lägg in det sovande objektet b in i object-listan med rörliga objekt
				objects.push_back( sleepies[b] );

				// Sedan plockar vi bort objekt b från sleepies
				sleepies[b] = sleepies.back();
				sleepies.pop_back();
			}
		}
	}


	//for(size_t a=0; a<objects.size(); ++a)
	//{
	//	// Test if someone should start to sleep
	//	float totalSpeedQuadratic = dot(objects[a].vel,objects[a].vel);
	//	if( totalSpeedQuadratic < 0.001f )
	//	{
	//		// object[a] sover, och skall läggas in i sleepies-listan
	//		sleepies.push_back( objects[a] );
	//		objects[a] = objects.back();
	//		objects.pop_back();
	//	}
	//}
}

void PhysB::draw(){
	for(size_t i=0; i<objects.size(); ++i)
		objects[i].draw();

	for(size_t i=0; i<sleepies.size(); ++i)
		sleepies[i].draw();
	
	int k = 10;
	float s = 0.5f;
	for(int i=-k; i<=k; ++i){
		drawLineFromTo(vec3(-k*s,0,i*s),vec3(k*s,0,i*s));
		drawLineFromTo(vec3(i*s,0,-k*s),vec3(i*s,0,k*s));
	}
}

void PhysB::destroy(){
	//...
}
