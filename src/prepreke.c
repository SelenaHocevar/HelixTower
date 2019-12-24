#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01


GLfloat ambient_coeffs[] = { 0.5, 0.2, 0.2, 1 };
GLfloat diffuse_coeffs[] = { 0.5, 0.2, 0.2, 1 };
GLfloat specular_coeffs[] = { 1, 0, 0, 1 };
GLfloat shininess = 3;


void nacrtaj_zid_1(float angle,float kraj_zida,int i){

  glPushMatrix();

  glRotatef(angle*abs(i),0,1,0);

  glBegin(GL_TRIANGLE_FAN);

  glVertex3f(0,i,0);
  for (float v = 0; v <= kraj_zida; v += PI / 20)
          glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
  glEnd();
  glPopMatrix();
}


void nacrtaj_zid_2(float angle,float kraj_zida,int i){

  glPushMatrix();

  glRotatef(angle*abs(i),0,1,0);

  glBegin(GL_TRIANGLE_FAN);

  glVertex3f(0,i,0);
  for (float v = 0; v <= 5*PI/6; v += PI / 20) {
          glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
  }
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0,i,0);
  for (float v = PI; v <= kraj_zida; v += PI / 20) {
          glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
  }
  glEnd();

  glPopMatrix();
}


void nacrtaj_horizontalnu_prepreku(int i,float pocetak_prepreke,float kraj_prepreke){
    glPushMatrix();

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glShadeModel(GL_SMOOTH);

        glRotatef(30*abs(i),0,1,0);


        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0,i+0.01,0);
            for (float v = pocetak_prepreke; v <= kraj_prepreke+ EPSILON; v += PI / 20) {
                    glVertex3f(sin(v)*2.5,i+0.01,cos(v)*2.5);
            }

        glEnd();

    glPopMatrix();
}

void nacrtaj_vertikalnu_prepreku(int i){
  glPushMatrix();
    glTranslatef(1.5,i+0.4,1);
    glRotatef(-45,0,1,0);
    glScalef(1.5,1,0.2);
    glutSolidCube(0.8);
  glPopMatrix();
}
