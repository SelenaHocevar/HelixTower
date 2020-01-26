#ifndef __FUNKCIJE__
#define __FUNCKIJE__

void set_normal_and_vertex(float u,float v);
void set_normal_and_vertexSphere(float u, float v);
void draw_Sphere();
void draw_Cylinder();
void draw_Cone();
void nacrtaj_bodlje(int i,float pocetak_prepreke,int prosao_kroz_portal);
void nacrtaj_vertikalnu_prepreku(int i,float animation_parametar);
void nacrtaj_zid_1(float angle,float kraj_zida,int i);
void nacrtaj_zid_2(float angle,float pocetak_prvog,float kraj_prvog,float pocetak_drugog,float kraj_drugog,int i);
void ispisi_rezultat(int score);
void game_over(float y_loptica);
void you_win();
void legend();
void nacrtaj_okvir_1(float angle,float kraj_zida,int i);
void nacrtaj_portal(int i);



#endif
