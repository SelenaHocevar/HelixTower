#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01


GLfloat ambient_coeffs[] = { 0.9, 0.5, 0.2, 1 };
GLfloat diffuse_coeffs[] = { 0.9, 0, 0, 1 };
GLfloat specular_coeffs[] = { 1, 0, 0, 1 };
GLfloat shininess = 3;

void nacrtaj_horizontalni_zidic(int i){
    glPushMatrix();
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glShadeModel(GL_SMOOTH);
        
        glRotatef(30*abs(i),0,1,0);
        
        
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0,i+0.01,0);
            for (float v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
                
                if(v>=PI+PI/2 && v<=PI+5*PI/6)
                    glVertex3f(sin(v)*2.5,i+0.01,cos(v)*2.5);
            }
            
        glEnd();
        
    glPopMatrix();
}
