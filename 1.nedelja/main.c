#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define EPSILON 0.01


static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

int main(int argc, char **argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    
    glEnable(GL_NORMALIZE);

    
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat model_ambient[] = { 0.7, 0.5, 0.9, 1 };
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;

    }
}

static void on_reshape(int width, int height)
{
   
    glViewport(0, 0, width, height);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 15);
}





void set_normal_and_vertex(float u, float v)
{
    
    glNormal3f(
            sin(v),
            0,
            cos(v)
            );
    glVertex3f(
            sin(v),
            u,
            cos(v)
            );
}


void set_normal_and_vertexSphere(float u, float v)
{
    glNormal3f(
            sin(u) * sin(v),
            cos(u),
            sin(u) * cos(v)
            );
    glVertex3f(
            sin(u) * sin(v)/6,
            cos(u)/6,
            sin(u) * cos(v)/6
            );
}


void draw_Sphere()
{
    float u, v;

    glPushMatrix();
    
    glTranslatef(0,-3,0);
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



void draw_Cylinder()
{
    float u, v;

    glPushMatrix();

    glTranslatef(0,-6,0);
    for (u = 0; u < PI*6; u += PI / 200) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }

    
    glPopMatrix();
}

static void on_display(void)
{
    
    GLfloat light_position[] = { 1, 1, 1, 0 };

   
    GLfloat ambient_coeffs[] = { 0.5, 1, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { 0.4, 0, 0.7, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    
    GLfloat ambient_coeffsBall[] = { 0.9, 0.2, 0.3, 1 };
    GLfloat diffuse_coeffsBall[] = { 0.5, 0, 0, 1 };
    GLfloat specular_coeffsBall[] = { 1, 1, 1, 1 };

    GLfloat shininess = 30;

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0.8, 5, 0, 0.5, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glPushMatrix();  
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glShadeModel(GL_SMOOTH);
        draw_Cylinder();
    glEnd();

    
    
   glPushMatrix();
        glTranslatef(0, 6, 1);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsBall);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        glShadeModel(GL_SMOOTH);
        draw_Sphere();
    glPopMatrix();
   

    
    glutSwapBuffers();
}

