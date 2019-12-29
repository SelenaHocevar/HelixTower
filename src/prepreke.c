#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01



/*Funkcija koja crta prvu vrstu zidova
 * kraj_zida odredjuje koje ce velicine biti rupa
 */
void nacrtaj_zid_1(float angle,float kraj_zida,int i){
    
    glPushMatrix();
    
    //Ceo zid se rotira za odredjen ugao
    glRotatef(angle*abs(i),0,1,0);
    
    //Crtaju se krugovi koji predstavljaju zidove
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,i,0);
    for (float v = 0; v <= kraj_zida; v += PI / 20){
        glTexCoord2f(sin(v)*0.4+0.5, cos(v)*0.4+0.5);
        glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
    }
    glEnd();
    glPopMatrix();
}

 
void lava(float angle,float pocetak_zida,float kraj_zida,int i){
    
    glPushMatrix();
    
    //Ceo zid se rotira za odredjen ugao
    glRotatef(angle*abs(i)*3,0,1,0);
    
    //Crtaju se krugovi koji predstavljaju zidove
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,i+0.1,0);
    for (float v = pocetak_zida; v <= kraj_zida; v += PI / 20){
        glTexCoord2f(sin(v)*1+0.5, cos(v)+0.5);
        glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
    }
    glEnd();
    glPopMatrix();
}




/*Funkcija za crtanje okvira za prvu vrstu zida*/
void nacrtaj_okvir_1(float angle,float kraj_zida,int i){
    glPushMatrix();
    
    //Postavlja se boja
    glColor3f(0.8,0.8,0.8);
    
    //Postavlja se debljina linije
    glLineWidth(3);
    
    //Rotira se da bi se poklopila sa zidom
    glRotatef(angle*abs(i),0,1,0);
    
    //Crta se linija
    glBegin(GL_LINE_STRIP);
    for (float v = 0; v <= kraj_zida; v += PI / 20)
        glVertex3f(sin(v)*2.501,i,cos(v)*2.501);
    glEnd();
    
    glPopMatrix();
}


void nacrtaj_zid_2(float angle,float pocetak_prvog,float kraj_prvog,float pocetak_drugog,float kraj_drugog,int i){
    
    glPushMatrix();
    
    glRotatef(angle*abs(i),0,1,0);
    
    glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,i,0);
    for (float v = pocetak_prvog; v <= kraj_prvog; v += PI / 20) {
        glTexCoord2f(sin(v)*0.4+0.5, cos(v)*0.4+0.5);
        glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
    }
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,i,0);
    for (float v = pocetak_drugog; v <= kraj_drugog; v += PI / 20) {
        glTexCoord2f(sin(v)*0.4+0.5, cos(v)*0.4+0.5);
        glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
    }
    glEnd();
    
    glPopMatrix();
}


/* Funkcija za crtanje bodlji */
void nacrtaj_bodlje(int i,float pocetak_prepreke,float kraj_prepreke){
    glPushMatrix();
    
    glRotatef(30*abs(i),0,1,0);
    
    // Crta se po 5 bodlji u 3 reda
    for(float k =1.2;k<=2.5;k+=0.6)
        for(float j=0.1;j<1;j+=0.2){
            glPushMatrix();
            
            glTranslatef(sin(pocetak_prepreke+j)*k,i,cos(pocetak_prepreke+j)*k);
            
            // Crta se kupa koja predstavlja bodlju
            glRotatef(-90,1,0,0);
            glColor3f(0.47,0.47,0.48);
            glutSolidCone(0.06,0.4,30,30);
            
            // Crta se crvena zicana kupa koja predstavlja krv
            glColor3f(0.6,0.1,0.1);
            glutWireCone(0.048,0.405,4,0);
            
            glColor3f(0.55,0.55,0.55);
            glutWireCone(0.07,0.35,2,2);
            glPopMatrix();
        }
        
        glPopMatrix();
}

void nacrtaj_vertikalnu_prepreku(int i,float animation_parameter){
    glPushMatrix();
    
    
    double clip_plane[] = {0, -1, 0, animation_parameter+i};
    
    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);
    
    
    
    glTranslatef(1.4,i+0.9,1);
    glRotatef(-45,0,1,0);
    glScalef(1.8,2.4,0.2);
    glColor3f(0.5,0.5,0.5);
    glutSolidCube(0.8);
    
    
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}


void nacrtaj_veliku_bodlju(int i){
    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_LINES);
        glVertex3f(0,i+0.7,1);
        glVertex3f(0,i+0.5,2.6);
    glEnd();
    
    glBegin(GL_LINES);
        glVertex3f(0.2,i+0.5,1);
        glVertex3f(0,i+0.5,2.6);
    glEnd();
    
    glBegin(GL_LINES);
        glVertex3f(-0.2,i+0.5,1);
        glVertex3f(0,i+0.5,2.6);
    glEnd();
    
    glTranslatef(0,i+0.5,0);
    
  
        
    glColor3f(0.5,0.5,0.5);
    glutSolidCone(0.3,2.6,30,30);
    
    glPopMatrix();
}




