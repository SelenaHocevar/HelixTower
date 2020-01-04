#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01

/*Pomocna funkcija za crtanje valjka*/
void set_normal_and_vertex(float u, float v)
{

    glNormal3f(
            sin(v),
            0,
            cos(v)
            );
        
    glTexCoord2f(sin(v)*0.5+0.5, u*0.5+0.5);
    glVertex3f(
            sin(v),
            u,
            cos(v)
            );
}

/*Pomocna funkcija za crtanje lopte*/
void set_normal_and_vertexSphere(float u, float v)
{
    glNormal3f(
            sin(u) * sin(v),
            cos(u),
            sin(u) * cos(v)
            );
    glTexCoord2f(sin(u) * sin(v)/6*0.1+0.5, sin(u) * cos(v)/6*0.1+0.5);
    glVertex3f(
            sin(u) * sin(v)/6,
            cos(u)/6,
            sin(u) * cos(v)/6
            );
}

/*Funkcija za crtanje lopte*/
void draw_Sphere()
{
    float u, v;

    glPushMatrix();

    //glTranslatef(0,-3,0);
    for (u = 0; u < PI*2; u += PI / 200) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
            set_normal_and_vertexSphere(u, v);
            set_normal_and_vertexSphere(u + PI / 20, v);
        }
        glEnd();
    }


    glPopMatrix();
}


/*Funkcija za crtanje valjka*/
void draw_Cylinder()
{
    float u, v;

    glPushMatrix();

    glTranslatef(0,-10,0);
    for (u = 0; u < PI*18; u += PI / 20) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }


    glPopMatrix();
}

