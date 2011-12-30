#include <windows.h>		// Header File For Windows
#include <math.h>			// Math Library Header File
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <glm\glm.hpp>
#include <vector>
#include <time.h>
const double PI = 3.1415926535897;
using namespace glm;
using namespace std;

GLUquadric* sphereData;

float randUnit(){
	return float(rand())/RAND_MAX;
}

void drawCylinder(float* p, float rb, float rt , float rh, float startAngle=0.0f)
{
	size_t k = 4;
	float diff = (float)((2*PI)/k);

	glBegin(GL_QUAD_STRIP);
	float f = startAngle;
	for(size_t i=0; i<=k; ++i , f+=diff)
	{
		float x = (float)sin(f);
		float z = (float)cos(f);

		float p1[] = { p[0]+rt*x , p[1]+rh , p[2]+rt*z };
		glVertex3fv( p1 );
		float p2[] = { p[0]+rb*x , p[1]-rh , p[2]+rb*z };
		glVertex3fv( p2 );
	}
	glEnd();
}

class Sphere{
public:
	vec3 pos;
	vec3 vel;
	vec3 acc;
	vec3 col;
	float r;
	float m;
	Sphere( float r = 1.0f, float m = 1.0f, vec3 pos = vec3(0.0f), vec3 vel = vec3(0.0f), vec3 acc = vec3(0.0f), vec3 col = vec3( randUnit(), randUnit(), randUnit() ) )
		: r(r), m(m), pos(pos), vel(vel), acc(acc), col(col){
	}
	void draw() const{
		glColor3f( col.x , col.y , col.z );
		glTranslatef(  pos.x ,  pos.y ,  pos.z );
		gluSphere( sphereData , r , 16 , 16 );
		glTranslatef( -pos.x , -pos.y , -pos.z );
		glColor3f( 1.0f,1.0f,1.0f );
		/*
		int varv = 6;
		const float PH = PI/2;
		
		float a = -PH;
		float s = PH / varv;
		for(int i=0; i<=2*varv; ++i , a+=s)
		{
			float p[] = { pos.x , pos.y+r*a/PH , pos.z };
			float nb = sqrt(cos(a));
			float nt = sqrt(cos(a+s));
			
			drawCylinder( p , r*nb , r*nt , s*r/PI , PH/2 );
		}*/
	}
};

vector< Sphere > objects;

void PhysB_init()
{
	srand( time(0) );
	sphereData = gluNewQuadric();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for(int i=0; i<50; ++i)
	{
		Sphere s( 0.2f, 1.0f, vec3( 10*randUnit(), 10*randUnit(), 0*randUnit() ) );
		objects.push_back(s);
	}
}

void PhysB_update()
{
	float bounceCoefficient = 0.8f;
	float gravity = -0.003f;

	for(size_t a=0; a<objects.size(); ++a)
	{
		// Add more acceleration from the gravity
		objects[a].acc.y += gravity;

		// Kollidera kloten med varandra
		for(size_t b=0; b<objects.size(); ++b)
		{
			// Testa ej med sig själv
			if( a != b )
			{
				// Distansvektorn, dvs vart är object b gentemot objekt a
				vec3 dir = objects[a].pos - objects[b].pos;

				// Distansen i kvadrat
				float distQuadratic    = dot(dir,dir);

				// Minimumdistansen
				float minDistQuadratic = objects[a].r + objects[b].r;
				minDistQuadratic *= minDistQuadratic;

				// Så.. om det är en kollision mellan de två kloten a och b
				if( distQuadratic <= minDistQuadratic )
				{
					// normalisera dir så får vi normalen som visar hur de två kloten kolliderade
					// Vilka punkter på kloten var det som nuddade varandra
					vec3 norm = normalize(dir);

					objects[a].col = vec3( 1.0f, 1.0f, 1.0f );
					objects[b].col = vec3( 1.0f, 1.0f, 1.0f );
					//objects[a].acc.y = -objects[a].acc.y;
					//objects[b].acc.y = -objects[b].acc.y;
				}
			}
		}


		// World-Floor-collision
		if( objects[a].pos.y +  objects[a].acc.y > 0 )
		{
			objects[a].pos.y += objects[a].acc.y;
		}
		else
		{
			objects[a].acc.y *=- bounceCoefficient;
		}

		// Test if someone should start to sleep
		if( objects[a].acc.y < gravity )
		{
			// Stoppa object[a] in i en sleep-lista eller något
		}
	}
}

void PhysB_draw(){
	for(size_t i=0; i<objects.size(); ++i)
		objects[i].draw();
}
void PhysB_destroy(){
	//...
}