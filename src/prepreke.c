#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535

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


/*Funkcija koja crta dva odvojena zida na istoj visini*/
void nacrtaj_zid_2(float angle,float pocetak_prvog,float kraj_prvog,float pocetak_drugog,float kraj_drugog,int i){
    
    glPushMatrix();
    
    //Ceo zid se rotira za odredjen ugao
    glRotatef(angle*abs(i),0,1,0);
    
    //Prvi zid
    glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,i,0);
    for (float v = pocetak_prvog; v <= kraj_prvog; v += PI / 20) {
        glTexCoord2f(sin(v)*0.4+0.5, cos(v)*0.4+0.5);
        glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
    }
    
    glEnd();
    
    
    //Drugi zid
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


/* Funkcija za crtanje bodlji */
void nacrtaj_bodlje(int i,float pocetak_prepreke,int prosao_kroz_portal){
    glPushMatrix();
    
    glRotatef(30*abs(i), 0, 1, 0);
    
    /* Crta se po 5 bodlji u 3 reda */
    for(float k = 1.2; k <= 2.5; k += 0.6)
        for(float j = 0.1; j < 1; j += 0.2){
            glPushMatrix();
            
            glTranslatef(sin(pocetak_prepreke + j)*k, i, cos(pocetak_prepreke + j)*k);
            
            /* Crta se kupa koja predstavlja bodlju */
            glRotatef(-90, 1, 0, 0);
            
            /*Menja se boja u zavisnosti od toga da li je loptica prosla kroz portal ili ne*/
            if(!prosao_kroz_portal)
                glColor3f(0.45, 0.45, 0.45);
            else
                glColor3f(0.1, 0.1, 0.1);
            
            glutSolidCone(0.06, 0.4, 30, 30);
            
            /* Crta se crvena zicana kupa koja predstavlja krv */
            glColor3f(0.6, 0.1, 0.1);
            glutWireCone(0.048, 0.405, 4, 0);
            
            if(!prosao_kroz_portal)
                glColor3f(0.55, 0.55, 0.55);
            else
                glColor3f(0.25, 0.25, 0.25);
            
            glutWireCone(0.07, 0.35, 2, 2);
            
            glPopMatrix();
        }
        
    glPopMatrix();
}


/* Funkcija za crtanje portala  */
void nacrtaj_portal(int i){
    glPushMatrix();
    
    /*Svaki sledeci portal je rotiran za jos 30 stepeni*/
    glRotatef(30*i,0,1,0);
    
    /*Pozicioniramo ga i skaliramo*/
    glTranslatef(0,i+0.8,1.8);
    glRotatef(90,0,1,0);
    glRotatef(-90,1,0,0);
    
    /*Skaliramo ga da bude elipsa*/
    glScalef(0.3,1,0.3);
    
    /*Crtamo krug i podesavamo teksture*/
    glBegin(GL_TRIANGLE_FAN);
    
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0,0,0);
    for (float v = 0; v <= 2*PI+PI/6; v += PI / 20) {
        glTexCoord2f(sin(v)*0.4+0.7, cos(v)*0.4+0.5);
        glVertex3f(sin(v)*2.5,0,cos(v)*2.5);
    }
    
    glEnd();
    
    glPopMatrix();
}

