#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01


//Funkcija za ispisivanje trenutnog rezultata
void ispisi_rezultat(int score){
    glColor3f(0.9, 0.1, 0.1);
 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    char score_s[50];
    sprintf(score_s,"Score:  %d",score);

   
    glRasterPos2i(70, 280);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,
                 score_s);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


//Funkcija koja ispisuje na ekran poruku za kraj igre
void game_over(float y_loptica){
    
    glPushMatrix();
    
    glColor3f(1,1,1);
    
    glPushMatrix();
    
    //Pravougaonik sa porukom se pozicionira na sredinu
    glTranslatef(0,0.5,0);
    glScalef(0.5,0.3,1);
    
    //Crta se beli pravougaonik na kojem ce se nalaziti tekst
    glBegin(GL_POLYGON);
    glVertex3f(-2,y_loptica+2,1);
    glVertex3f(2,y_loptica+2,1);
    glVertex3f(2,y_loptica-2,4);
    glVertex3f(-2,y_loptica-2,4);
    glEnd();
    
    //Crta se okvir pravougaonika
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2,y_loptica+2,1);
    glVertex3f(2,y_loptica+2,1);
    glVertex3f(2,y_loptica-2,4);
    glVertex3f(-2,y_loptica-2,4);
    glEnd();
    
    glPopMatrix();
    
    //Postavlja se boja teksta
    glColor3f(0.9, 0.1, 0.1);
    
 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 400, 0, 400);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

   
    glRasterPos2i(170, 280);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,
                 "   GAME OVER\nPress R to try again!");
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopMatrix();
}


//Funkcija koja u gornjem desnom uglu ispisuje osnovne informacije i kontrole
void legend(){
    //Postavlja se boja teksta
    glColor3f(0.9, 0.1, 0.1);
 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //Pozicionira se i ispisuje poruka
    glRasterPos2i(230, 280);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,
                 "Press G to start\nControls:A,D");
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
