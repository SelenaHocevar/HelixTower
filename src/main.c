#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

#define FILENAME0 "../Textures/sky3.bmp"
#define FILENAME1 "../Textures/brick2.bmp"
#define FILENAME2 "../Textures/floor.bmp"
//#define FILENAME3 "../Textures/lava.bmp"

#define PI 3.1415926535
#define EPSILON 0.01
#define TIMER_ID 0
#define TIMER_INTERVAL 20

static float phi, pocetak, kraj, angle;
static float y_loptica, v_y, visina_lopte;
static int u = 0, d = 1, animation_ongoing, p, score=0, k, gameover;
static float kraj_zida[15], matrix[16];
char buffer[20];
static GLuint names[4];
float animation_param,v_param;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
void delay(int s);
static void textures(void);


int main(int argc, char **argv)
{

    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(2000, 2000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);


    /* Parametri svetla */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat model_ambient[] = { 0.7, 0.7, 0.7, 1 };

    glEnable(GL_LIGHTING);

    /* Postavljaju se svojstva prvog svetla */
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    glClearColor(1,1,1,1);
    
    /*Brzina loptice*/
    v_y=0.1;

    /*Pocetna pozicija loptice*/
    y_loptica = 27;

    /*Pozicija prvog zida*/
    p=26;

    /**/
    visina_lopte =1;
    
    /*Indikator da li je igrac izgubio*/
    gameover=0;

    /*Pomocni niz za random generisanje zidova*/
    srand(time(0));
    for(int j=0;j<13;j++)
      kraj_zida[j] = PI+(rand() % 4+2)*(PI/6);
    k=0;
    
    
    /*Teksture*/
    textures();

    animation_param=0;
    v_param=0.05;

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}


static void textures(void)
{
    /* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

    /* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /*
     * Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz
     * fajla.
     */
    image = image_init(0, 0);

    /* Kreira se prva tekstura. */
    image_read(image, FILENAME0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(4, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    image_read(image, FILENAME1);
    glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME2);
    glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

/*
    image_read(image, FILENAME3);
    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);*/
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}


static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'a':
        case 'A':
            /*Rotacija zidova ulevo*/
            phi-=15;
            if(phi<=0)
                phi +=360;
            
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            /*Rotacija zidova udesno*/
            phi+=15;
            if(phi>=360)
                phi -=360;
            
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            /*Start*/
            if(!animation_ongoing){
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
                animation_ongoing = 1;
            }
            break;
        case 'f':
            animation_ongoing=0;
            break;
        case 'r':
        case 'R':
            /*Igra se resetuje*/
            y_loptica =27 ;
            p=26;
            phi=0;
            score=0;
            k=0;
            gameover=0;
            glutPostRedisplay();
            break;
    }
}

static void on_timer(int value){
    
    if(value != TIMER_ID)
        return ;
    
    animation_param+=v_param;
    if(animation_param >= 2)
        v_param*=-1;
    if(animation_param <= 0)
        v_param*=-1;
    
    /*Smanjuje se y koor loptice tj. ona pada*/
    y_loptica -=v_y;
    
    
    /*Parametar koji pomera pozicije kada igrac pomera zidove
     *      phi- ugao koji se menja kada igrac pritiska w ili a
     *      p - y pozicija zida
     *      angle - svaki zid je na pocetku pomeren za neki ugao u zavisnosti od p
     */
    float m = phi+abs(p)*angle;
    if(m>=360)
        m-=360;
    
    
    /*Pozicije prepreke na osnovu kojih se odredjuje da li je loptica udarila u nju ili ne*/
    float pocetak_prepreke = sin(pocetak + m*2*PI/360) * 2.5;
    float kraj_prepreke = sin(kraj + m*2*PI/360) * 2.5;
    
    /*Pocetna i krajnja pozicija mesta kroz koje loptica treba da prodje*/
    float pocetak_rupe = sin(kraj_zida[k] + m*2*PI/360) * 2.5;
    float kraj_rupe = sin(m*2*PI/360) * 2.5;
    
    
    /*Ako se loptica krece nadole*/
    if(d){
        visina_lopte = 1;
        v_y += 0.0005;
        
        if(y_loptica <= p + 0.2 ){
            
            // Uslov koji proverava da li je loptica udarila u bodlje
            if(pocetak_prepreke < 0 && kraj_prepreke > 0.5 ){ 
                delay(500);
                animation_ongoing = 0;
                gameover = 1;
                
            // Uslov za prolazak na sledeci zid
            }else if(pocetak_rupe <= 0 && kraj_rupe > 0){ 
                p-=3;
                score++;
                k++;
                
            // Loptica je udarila u zid i odbila se
            }else{ 
                visina_lopte=0.75;
                v_y=0.1;
                v_y*=(-1);
                u= 1;
                d= 0;
            }
        }
    
    /*Ako se loptica krece nagore*/
    }else if(u){
        visina_lopte=1;
        if(y_loptica >= p+1.6 ){
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
    gluPerspective(90, (float) width / height, 1, 150);
}






static void on_display(void)
{

    /*Pozicija svetla*/
    GLfloat light_position[] = { 0, 1, 1, 0 };


    /* Parametri materijala za lopticu */
    GLfloat ambient_coeffsBall[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat diffuse_coeffsBall[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat specular_coeffsBall[] = { 0.3, 0.3, 0.2, 1 };
    GLfloat shininess = 30;
    

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Inicijalizuje se matrica transformacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0.5+ y_loptica, 5.5, 0, -3.5+y_loptica, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMultMatrixf(matrix);

    
    glPushMatrix();
    
    /*Pozicioniramo pozadinu*/
    glTranslatef(0,y_loptica-20,-2);
    glScalef(10,10,1);
    glRotatef(10,1,0,0);
    
    /* Ukljucujemo teksturu za pozadinu */
    glBindTexture(GL_TEXTURE_2D, names[0]);
    
    /* Crtamo pozadinu */
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0.1, 0);
        glVertex3f(3.3, 0, 0);

        glTexCoord2f(1, 0);
        glVertex3f(-3.3, 0, 0.01);

        glTexCoord2f(1, 1);
        glVertex3f(-3.3, 2.2, 0.01);

        glTexCoord2f(0.1, 1);
        glVertex3f(3.3, 2.2, 0);
    glEnd();
    glPopMatrix();



    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /*Crtanje valjka i prepreka*/
    glPushMatrix();
    glRotatef(phi,0,1,0);
    glPushMatrix();
    glRotatef(-phi,0,1,0);
    glBindTexture(GL_TEXTURE_2D, names[1]);
    draw_Cylinder();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);




    angle =30;
    int j=12;
    
    
    /*Crtaju se zidovi*/
    for(int i = -10;i<= 26;i+=3){
        
        glBindTexture(GL_TEXTURE_2D, names[2]);
            nacrtaj_zid_1(angle,kraj_zida[j],i);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        nacrtaj_okvir_1(angle,kraj_zida[j],i);
        
        j--;
    }

    
    // Crtaju se bodlje
    pocetak = PI;
    kraj = PI+PI*2/6;
    j=12;
    for(int i = -10;i<= 26;i+=3){
      nacrtaj_bodlje(i,pocetak,kraj);
      if(i == 23)
      nacrtaj_vertikalnu_prepreku(i,animation_param);
      
    }

    glPopMatrix();




    /*Crtanje loptice*/
    glPushMatrix();
    /* Loptica se krece po y osi u zavisnosti od parametra y_loptica */
    glTranslatef(0, y_loptica, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsBall);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    glScalef(1,visina_lopte,1);
    draw_Sphere();
    glPopMatrix();


    
    /* Ispisuje se trenutni rezultat. */
    ispisi_rezultat(score);
    
    /* Ispisuju se osnovne informacije. */
    legend();
    
    /* Ako je igrac izgubio ispisuje se odgovarajuca poruka. */
    if(gameover)
        game_over(y_loptica);
    
 
    /* Nova slika se salje na ekran. */
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
