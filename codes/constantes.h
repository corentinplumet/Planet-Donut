#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

constexpr double dim_max(1000.);                    // km
constexpr double rayon_min(50.);                    // km
constexpr double rayon_max(150.);                   // km
constexpr double display_rayon_robot(0.15);         // km
constexpr double rayon_base(1);                     // km
constexpr double rayon_comm(300.);                  // km

constexpr int dg = ((2*dim_max)/rayon_comm) + 1;
constexpr int cases = dg*dg;

constexpr double deltaD(5.);                        // km
constexpr double maxD_prosp(10 * dim_max);          // km
constexpr double maxD_forage(1.42 * dim_max);       // km
constexpr double maxD_transp(5 * dim_max);          // km
constexpr double maxD_com(1.42 * dim_max);          // km
constexpr unsigned int max_robots(3);               // km

constexpr double iniR(1000);                        // ktonne
constexpr double finR(10*iniR);                     // ktonne
constexpr double deltaR(iniR/4.);                   // ktonne
constexpr double cost_repair(0.0005);               // ktonne/km
constexpr double cost_prosp(iniR/100.);             // ktonne
constexpr double cost_forage(iniR/10.);             // ktonne
constexpr double cost_transp(iniR/10.);             // ktonne
constexpr double cost_com(iniR/1000.);              // ktonne

constexpr unsigned taille_dessin(800);
constexpr unsigned max_line(200);

enum Etat_lecture {NB0,GISEMENT,NB1,BASE,ROBOTBASE,PROSPECTION,FORAGE,TRANSPORT,
				   COMMUNICATION,FIN};

#endif // CONSTANTES_H_INCLUDED
