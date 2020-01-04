#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "funkcije.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "image.h"
#include <stdbool.h>

#define FILENAME0 "../Textures/sky1.bmp"
#define FILENAME1 "../Textures/brick2.bmp"
#define FILENAME2 "../Textures/floor.bmp"
#define FILENAME3 "../Textures/sky2.bmp"
#define FILENAME4 "../Textures/lava5.bmp"
#define FILENAME5 "../Textures/portal1.bmp"
#define FILENAME6 "../Textures/grass.bmp"

#define PI 3.1415926535
#define TIMER_ID 0
#define TIMER_INTERVAL 20

static float phi, pocetak, kraj, angle;
static float y_loptica, v_y, visina_lopte;
static int u = 0, d = 1, animation_ongoing, p, score = 0, k, gameover;
static float kraj_zida[15], matrix[16];
char buffer[20];
static GLuint names[4];
float animation_param,v_param;
float pocetak_zida_1;
float kraj_zida_1;
float pocetak_zida_2;
float kraj_zida_2;
int prosao_kroz_portal = 0;
int youwin = 0;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);
void delay(int s);
static void textures(void);


/*Pomocna funkcija za racunanje pozicije ivica zidova*/
float pozicija(float a,float m){
  return sin(a + m*2*PI/360) * 2.5;
}

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
    GLfloat light_specular[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat model_ambient[] = { 0.7, 0.7, 0.7, 1 };

    glEnable(GL_LIGHTING);

    /* Postavljaju se svojstva prvog svetla */
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    
    glClearColor(1, 1, 1, 1);

    
    /*Brzina loptice*/
    v_y = 0.15;

    /*Pocetna pozicija loptice*/
    y_loptica = 37;

    /*Pozicija prvog zida*/
    p = 36;

    /**/
    visina_lopte = 1;

    /*Indikator da li je igrac izgubio*/
    gameover = 0;

    /*Pomocni niz za random generisanje zidova*/
    srand(time(0));
    for(int j = 0;j < 13;j++)
      kraj_zida[j] = PI + (rand() % 1 + 4)*(PI/6);
    k = 0;


    /*Teksture*/
    textures();

    animation_param = 0;
    v_param = 0.05;

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
        case 'A':
            /*Rotacija zidova ulevo*/
            phi -= 15;
            if(phi <= 0)
                phi += 360;

            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            /*Rotacija zidova udesno*/
            phi += 15;
            if(phi >= 360)
                phi -= 360;

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
            animation_ongoing = 0;
            break;
        case 'r':
        case 'R':
            /*Igra se resetuje*/
            y_loptica = 37 ;
            p = 36;
            phi = 0;
            score = 0;
            k = 0;
            gameover = 0;
            youwin = 0;
            prosao_kroz_portal = 0;
            v_y = 0.15;
            u = 0;
            d = 1;
            glutPostRedisplay();
            break;
    }
}

static void on_timer(int value){

    if(value != TIMER_ID)
        return ;

    /*Parametar koji utice na podizanje i spustanje zidova*/
    animation_param+=v_param;
    if(animation_param >= 2)
        v_param*=-1;
    if(animation_param <= 0)
        v_param*=-1;
    
    
    /*Smanjuje se y koor loptice tj. ona pada*/
    y_loptica -= v_y;

    

    /*Parametar koji pomera pozicije kada igrac pomera zidove
     *      phi- ugao koji se menja kada igrac pritiska d ili a
     *      p - y pozicija zida
     *      angle - svaki zid je na pocetku pomeren za neki ugao u zavisnosti od p
     */
    float m = phi+abs(p)*angle;
    if(m >= 360)
        m -= 360;
    
    
    /*Ako se na trenutnom zidu nalazi vertikalna prepreka*/
//     if(abs(p%2)==1){
//         /*X pozicija vertikalnog zida*/
//         float poz_zida = sin(phi*2*PI/360) * 2.5;
//         printf("%.3f %.3f\n",poz_zida,phi);
//         
//         /*Ako je zid podignut vise od trenutne pozicije loptice ona se odbija od njega, inace ga preskace*/
//         if(poz_zida <= 0.6 && poz_zida >=-0.6 && animation_param > y_loptica-p)
//             if(poz_zida >= 0)
//                 phi-=30;
//             else
//                 phi+=30;
//     }
  
    
    if(abs(p%2) == 1){
        float poz_zida = sin(m*2*PI/360) * 2.5;
        float poz_zida_z = cos(m*2*PI/360) * 2.5;
        printf("%.3f\n",poz_zida);
        if(poz_zida <= 0.3 && poz_zida >= -0.3 && poz_zida_z > 0 )
            prosao_kroz_portal = 1;
    }
    
    bool t;
        if(p%2 == 0)
          t = (pozicija(kraj_zida_2, m) <= 0 && pozicija(pocetak_zida_1, m) > 0) || (pozicija(kraj_zida_1, m) <= 0 && pozicija(pocetak_zida_2, m) > 0);
        else
          t = pozicija(kraj_zida[k], m) <= 0 && pozicija(0, m) > 0;

    
    
    if(y_loptica <= -2){
        prosao_kroz_portal = 0;
        youwin = 1;
    }
    
    /*Ako se loptica krece nadole*/
    if(d){
        visina_lopte = 1;
    
        /*Trenutak kada loptica udari u zid*/
        if(prosao_kroz_portal && y_loptica <= p + 0.3 || y_loptica <= p + 0.2 ){

            //Uslov koji proverava da li je loptica udarila u bodlje
          if( pozicija(pocetak,m) < 0 && pozicija(kraj,m) > 0.5 && p >= 0 || prosao_kroz_portal && pozicija(PI/6,m) < 0 && pozicija(3*PI/6,m) > 0.5 && p >= 0){
                delay(500);
                animation_ongoing = 0;
                gameover = 1;

            //Uslov za prolazak na sledeci zid
          }else if(t){
                p -= 3;
                score++;
                k++;

            //Loptica je udarila u zid i odbila se
            }else{
                visina_lopte = 0.75;
                
                if(prosao_kroz_portal)
                   v_y=0.25; 
                
                v_y *= (-1);
                u = 1;
                d = 0;
            }
        }

    /*Ako se loptica krece nagore*/
    }else if(u){
        visina_lopte = 1;
        if(y_loptica >= p + 1.7 ){
            v_y *= (-1);
            d = 1;
            u = 0;
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

    /*Menja se pozadina u zavisnosti od toga da li je igrac prosao kroz portal*/
    if(!prosao_kroz_portal)
        glClearColor(1, 1, 1, 1);
    else{
        glClearColor(0, 0, 0, 0);
    }
    
    /* Parametri materijala za lopticu */
    GLfloat ambient_coeffsBall[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat diffuse_coeffsBall[] = { 0.4, 0.25, 0.25, 1 };
    GLfloat specular_coeffsBall[] = { 0.3, 0.3, 0.2, 1 };
    GLfloat shininess = 5;


    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Inicijalizuje se matrica transformacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /*Kamera se krece zajedno sa lopticom do trenutka dok ne dodje do kraja igre*/
    float kamera_param = y_loptica > 0 ? y_loptica : 0;
    float kamera_param_z = youwin == 0 ? 5.5 : 4;
    
    gluLookAt(0,0.5 + kamera_param+kamera_param_z - 5.5, kamera_param_z, 0, -3.5 + kamera_param, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMultMatrixf(matrix);


    /**************POZADINA****************/
    
    glPushMatrix();
    
    /*Pozicioniramo je i skaliramo*/
    if(!prosao_kroz_portal){
        
        /* Ukljucujemo teksturu*/
        glBindTexture(GL_TEXTURE_2D, names[0]);
        
        float pozadina_param = y_loptica > 0 ? y_loptica - 20 : -20;
        
        glTranslatef(0, pozadina_param + 0.2, -2.4);
        glScalef(10, 10, 1);
        glRotatef(10, 1, 0, 0);
    }else{
        
        float pozadina_param = y_loptica >= 0 ? y_loptica - 10 : -10;
        
        /* Ukljucujemo teksturu*/
        glBindTexture(GL_TEXTURE_2D, names[3]);
        glTranslatef(0, pozadina_param, -2);
        glScalef(9, 9, 1);
        glRotatef(-10, 1, 0, 0);
    }

    /* Crtamo pozadinu */
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);

        glTexCoord2f(0.1, 0);
        glVertex3f(3.3, 0, 0);

        glTexCoord2f(1, 0);
        glVertex3f(-3.3, 0, 0);

        glTexCoord2f(1, 1);
        glVertex3f(-3.3, 2.2, 0);

        glTexCoord2f(0.1, 1);
        glVertex3f(3.3, 2.2, 0);
    glEnd();
    glPopMatrix();

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /****************************************/



    glPushMatrix();

    
    /***********TORANJ***********/
    
    glPushMatrix();
    
    /*Pozicioniramo ga*/
    glTranslatef(0, 7, 0);
    
    /*Kada loptica prodje kroz portal toranj postaje crn inace ima teksturu*/
    if(!prosao_kroz_portal)
      glBindTexture(GL_TEXTURE_2D, names[1]);
    glColor3f(0, 0, 0);
    
    /*Crtamo valjak*/
    draw_Cylinder();
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
    
    /****************************/


    
    /******PLATFORMA NA DNU******/
    
    glPushMatrix();
    
    /*Platforma se ne vidi dok ne dodjemo do dna*/
    double clip_plane[] = {0, -1, 0, -y_loptica + 3};

    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);
    
    /*Postavljamo boju*/
    if(!prosao_kroz_portal){
         glBindTexture(GL_TEXTURE_2D, names[6]);
    
        /*Crtamo platformu*/
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-25, -3, 0);
        glTexCoord2f(4, 0);
        glVertex3f(25, -3, 0);
        glTexCoord2f(4, 4);
        glVertex3f(25, -3 ,8);
        glTexCoord2f(0, 4);
        glVertex3f(-25, -3, 8);
        glEnd();
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    glDisable(GL_CLIP_PLANE0);
    
    glPopMatrix();

    /****************************/
    
    
    
    /********************ZIDOVI*********************/
    
    /*Pomocni parametri*/
    angle = 30;
    int j = 12;
    pocetak_zida_1 = 0;
    kraj_zida_1 = PI/2;
    pocetak_zida_2 = PI;
    kraj_zida_2 = PI + 5*PI/6;
    
    /*Zidovi se okrecu pomocu phi koji kontrolise igrac*/
    glRotatef(phi, 0, 1, 0);
    
    /*Zidovi se crtaju na visini i, pomocu pomocnih funkcija iz prepreke.c*/
    for(int i = 0;i <= 36;i += 3){
        
        /*Postavljaju se teksture u zavisnosti od toga da li je loptica prosla kroz portal ili ne*/
        if(!prosao_kroz_portal)
            glBindTexture(GL_TEXTURE_2D, names[2]);
        else
            glBindTexture(GL_TEXTURE_2D, names[4]);
        
        
        if(i%2 == 0)
            nacrtaj_zid_2(angle, pocetak_zida_1, kraj_zida_1, pocetak_zida_2, kraj_zida_2, i);
        else{
            nacrtaj_zid_1(angle, kraj_zida[j], i);
            
            glBindTexture(GL_TEXTURE_2D, 0);
            if(!prosao_kroz_portal)
                nacrtaj_okvir_1(angle, kraj_zida[j], i);
        }
        
        /* Iskljucujemo aktivnu teksturu */
        glBindTexture(GL_TEXTURE_2D, 0);
        
        j--;  // brojac za prolazak kroz niz za random generisanje zidova
    }
    
    /*******************************************/
    
    

    /*********PREPREKE*********/
    
    pocetak = PI;
    kraj = PI + PI*2/6;
    
    for(int i = 0; i <= 36; i += 3){
        
        nacrtaj_bodlje(i, pocetak, kraj, prosao_kroz_portal);
        
        if(prosao_kroz_portal)
            nacrtaj_bodlje(i,PI/6,0 + 3*PI/6,prosao_kroz_portal);
        
        if(abs(i % 2) == 1){
            /* Ukljucujemo teksturu */
            glBindTexture(GL_TEXTURE_2D, names[5]);
            
            if(!prosao_kroz_portal)
                nacrtaj_portal(i, angle);
            
            /* Iskljucujemo aktivnu teksturu */
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    glPushMatrix();
    
    /* Crtamo portal na dnu tornja */
    
    glBindTexture(GL_TEXTURE_2D, names[5]);
    
    /* Da se ne bi okretao zajedno sa zidovima */
    glRotatef(-phi,0, 1, 0);
    
    /*Pozicioniramo ga i uvecavamo*/
    glTranslatef(2.7, -2, 5.5);
    glRotatef(90, 1, 0, 0);
    glScalef(1.5, 1.5, 1);
    
    /* Crtamo portal samo ako je loptica pre toga prosla kroz jedan */
    if(prosao_kroz_portal)
        nacrtaj_portal(-3, 0);
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPopMatrix();
    
    /**************************/

    glPopMatrix();




    /***********LOPTICA***********/
    
    glPushMatrix();
    
    /* Loptica se krece po y osi u zavisnosti od parametra y_loptica */
    glTranslatef(0, y_loptica, 2);
    
    /*Postavljanje materijala za lopticu*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffsBall);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffsBall);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffsBall);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glShadeModel(GL_SMOOTH);
    
    /*Oblik loptice se menja kada udari u zid*/
    glScalef(1,visina_lopte,1);
    
    glColor3f(0.5,0,0);
    /* Crta se loptica */
    draw_Sphere();
    
    glPopMatrix();
    
    /*****************************/

    
    /* Ispisuju se osnovne informacije. */
    legend();

    /* Ako je igrac izgubio ispisuje se odgovarajuca poruka. */
    if(gameover)
        game_over(y_loptica);
    
    /*Ako je igrac pobedio ispisuje se odgovorajuca poruka*/
    if(youwin)
        you_win(y_loptica);

    
    /* Ispisuje se trenutni rezultat. */
    ispisi_rezultat(score);

    
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
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
    glGenTextures(7, names);

    
    /* Nebo */
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

    /* Toranj */
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

    /* Zidovi */
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


    /* Crno nebo */
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
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    
    /* Lava */
    image_read(image, FILENAME4);
    
    glBindTexture(GL_TEXTURE_2D, names[4]);
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
    
    /* Portal */
    image_read(image, FILENAME5);
    
    glBindTexture(GL_TEXTURE_2D, names[5]);
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

    
    image_read(image, FILENAME6);
    
    glBindTexture(GL_TEXTURE_2D, names[6]);
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
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);

    /* Inicijalizujemo matricu rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
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
    while (clock() < start_time + milli_seconds);
}
