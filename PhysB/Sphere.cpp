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

#include "Sphere.h"

GLUquadric* sphereData = NULL;

float randUnit(){
	return float(rand())/RAND_MAX;
}

void drawCylinder(float* p, float rb, float rt , float rh, float startAngle)
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

void drawLineFromTo( vec3 from , vec3 to ){
	//glDisable(GL_DEPTH_TEST);
		glBegin(GL_LINES);
			glColor3f(1.0f,0.0f,0.0f);	glVertex3f(from.x,from.y,from.z);
			glColor3f(0.0f,0.0f,1.0f);	glVertex3f( to.x , to.y , to.z );
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
	//glEnable(GL_DEPTH_TEST);
}
void drawLineCenterVector( vec3 center , vec3 v){
	drawLineFromTo(center,center+v);
}


Sphere::Sphere( float r, float m, vec3 pos, vec3 vel, vec3 acc, vec3 col )
	: r(r), m(m), pos(pos), vel(vel), acc(acc), col(col){
}
void Sphere::draw() const{
	if(!sphereData)
		sphereData = gluNewQuadric();
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