#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"

#define PI 3.1415926535
#define EPSILON 0.01
#define TIMER_ID 0
#define TIMER_INTERVAL 20

static float phi,deltha_phi;
static float y_loptica,v_y;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
static int animation_ongoing;  

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
    v_y=0.05;
    
    
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
        glutPostRedisplay();
        break;
    case 'd':
        /*Rotacija valjka udesno*/
        phi+=6;
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
    gluLookAt(0, 4+y_loptica, 5.5, 0, 0.5+y_loptica, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glTranslatef(0,-2.5,0);
    
    
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
        for(int i = -10;i<= 5;i+=3){
            glPushMatrix();
                    glRotatef(30*i,0,1,0);
                    glBegin(GL_TRIANGLE_FAN);
                        glVertex3f(0,i,0);
                        for (float v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
                            //TODO: rand();
                            if(v<=PI+5*PI/6)
                                glVertex3f(sin(v)*2.5,i,cos(v)*2.5);
                        }
                    glEnd();
            glPopMatrix();
        }
        
    glPopMatrix();

    
    /*Crtanje loptice*/
   glPushMatrix();
        glTranslatef(0, 9+y_loptica, 2);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsBall);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glShadeModel(GL_SMOOTH);
        draw_Sphere();
    glPopMatrix();



    
    glutSwapBuffers();
}

