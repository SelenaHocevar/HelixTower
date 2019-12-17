#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"
#include <time.h>
#include <stdio.h>

#define PI 3.1415926535
#define EPSILON 0.01
#define TIMER_ID 0
#define TIMER_INTERVAL 20

static float phi;
static float y_loptica,v_y;
static int u = 0,d=1;
static int animation_ongoing;  
static int p;
static float positionP_x,positionP_z;
static float pocetna_pozicija_zidica;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
void delay(int s);



int main(int argc, char **argv)
{
    
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* Kreira se prozor. */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    
    /* Registruju se callback funkcije. */ 
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    
    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    
    
    
    /* Parametri svetla */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat model_ambient[] = { 0.7, 0.5, 0.9, 1 };
    
    glEnable(GL_LIGHTING);
    
    /* Postavljaju se svojstva prvog svetla */
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    /*Brzina loptice*/
    v_y=0.07;
    
    /*Pocetna poziija loptice*/
    y_loptica = 9;
    
    /*Pozicija prepreke*/
    p=8;
    
    /* Program ulazi u glavnu petlju. */
    glutMainLoop();
    
    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'a':
            /*Rotacija valjka ulevo*/
            phi-=6;
            if(phi<=0)
                phi +=360;
            glutPostRedisplay();
            break;
        case 'd':
            /*Rotacija valjka udesno*/
            phi+=6;
            if(phi>=360)
                phi -=360;
            glutPostRedisplay();
            break;
        case 'g':
            if(!animation_ongoing){
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                animation_ongoing = 1;
            }
            break;
        case 'f':
            animation_ongoing = 0;
            break;
    }
}

static void on_timer(int value){
    if(value != TIMER_ID)
        return ;
    
    /*Padanje loptice*/
    y_loptica -=v_y;
    
    
    float m = phi+abs(p)*30;
    if(m>=360)
        m-=360;
    
    pocetna_pozicija_zidica = sin(PI+PI/2)*2.5+sin(m*2*PI/360)*2.5;
    float positionZ_z = cos(PI+PI/2)*2.5+cos(m*2*PI/360)*2.5;
    
    
    positionP_x = sin(PI+5*PI/6)*2.5+sin(m*2*PI/360)*2.5 ;
    positionP_z = cos(PI+5*PI/6)*2.5+cos(m*2*PI/360)*2.5;

    float pocetak_rupe = sin(PI+5*PI/6)*2.5;
    
    /*Ako se loptica krece nadole*/
    if(d){
        if(y_loptica <= p+0.2 ){
            if(positionP_x > pocetak_rupe && positionP_x < 0 && positionP_z >=0){
                p-=3;
            }else if(pocetna_pozicija_zidica > pocetak_rupe && pocetna_pozicija_zidica<0 && positionZ_z >=0){
                delay(500);
                animation_ongoing = 0;
                y_loptica = 9;
                p=5;
                phi=0;
            }else{
                v_y*=(-1);
                u= 1;
                d= 0;
            }
        }
    /*Ako se loptica krece nagore*/
    }else if(u){
        if(y_loptica >= p+2 ){
            v_y*=(-1);
            d=1;
            u=0;
        }
    }
    
    
    
    
    glutPostRedisplay();
    
    if(animation_ongoing){
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);
    
    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 15);
}






static void on_display(void)
{
    
    /*Pozicija svetla*/
    GLfloat light_position[] = { 1, 1, 1, 0 };
    
    GLfloat ambient_coeffs[] = { 0.9, 1, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { 0.4, 0.9, 0.7, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    
    GLfloat ambient_coeffsP[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat diffuse_coeffsP[] = { 0.9, 0.8, 0.9, 1 };
    GLfloat specular_coeffsP[] = { 0, 0, 0, 1 };
    
    
    GLfloat ambient_coeffsBall[] = { 0.3, 0.4, 0.6, 1 };
    GLfloat diffuse_coeffsBall[] = { 0.9, 0.8, 0.9, 1 };
    GLfloat specular_coeffsBall[] = { 0, 0, 0, 1 };
    
    GLfloat shininess = 30;
    
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* Inicijalizuje se matrica transformacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0.5+ y_loptica, 5.5, 0, -3.5+y_loptica, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    
    /*Crtanje valjka i prepreka*/
    glPushMatrix();  
    glRotatef(phi,0,1,0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    draw_Cylinder();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsP);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsP);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsP);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    
    
    /*Prepreke*/
    for(int i = -10;i<= 8;i+=3){
        glPushMatrix();
        
        glRotatef(30*abs(i),0,1,0);
        
        glBegin(GL_TRIANGLE_FAN);
        
        glVertex3f(0,i,0);
        for (float v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
            
            if(v<=PI+5*PI/6)
                glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
        }
        
        glEnd();
        
        glPopMatrix();
        
    }
    
    
    for(int i = -10;i<= 8;i+=3){
        nacrtaj_horizontalni_zidic(i);
    }
    
    glPopMatrix();
    
    
    
    
    /*Crtanje loptice*/
    glPushMatrix();
    glTranslatef(0, y_loptica, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsBall);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    draw_Sphere();
    glPopMatrix();
    
    
    
    
    glutSwapBuffers();
}

/* Prekopirano sa sajta GeeksForGeeks
 *    Iskorisceno pri resetovanju igre */
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
    
    // Stroing start time 
    clock_t start_time = clock(); 
    
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
