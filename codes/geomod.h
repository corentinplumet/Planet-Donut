#ifndef GEOMOD_H_INCLUDED
#define GEOMOD_H_INCLUDED

struct Vecteur
{
	double x ;
	double y ;
} ;

struct Point
{
	double x ;
	double y ;
};

struct Cercle
{
	 Point centre ;
	 double rayon ;
};

double getter_epsilon_zero() ;

bool equal_zero(double b) ;

void setter_max(double a) ;

double getter_max() ;

Point normalise_c(Point& p) ; 

double normalise_c(double coordonne) ;

double construction_vecteur(Point a, Point b, Vecteur& z) ;

double construction_vecteur(Point a, Point b) ;
 
bool egalite(Point a, Point b) ;

bool dans_le_cercle(Point a, Cercle b) ;
 
bool inter_cercle(Cercle a, Cercle b) ;

void draw_black_disc(double x, double y, double r, double ressource);

void draw_colored_circle(double x, double y, int num_base);

void draw_colored_target(double x, double y,  int num_base);

void draw_colored_wagon(double x, double y,  int num_base, bool b);

void draw_colored_wifi_logo(double x, double y,  int num_base);

void draw_colored_triangle(double x, double y,  int num_base);

void draw_lign(Point a, Point b);

void draw_grey_circle(double x, double y);

class Rebouclement 
{
public :
	double getter_v() ;
	double getter_vm() ;
	double getter_vp() ;
	void setter_v(double a) ;
	void v_equivalent() ;
	
private :
	double v ;
	double vm ;
	double vp ;
} ;

#endif // GEOMOD_H_INCLUDED
