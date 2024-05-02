//~ geomode.cc Corentin Plumet et Eliot Goncalves MTBA2
#include <iostream>
#include <cmath>
#include "geomod.h"
#include "graphic.h"
#include "constantes.h"

namespace 
{
	double maxe(0) ;
	double epsilon_zero(0) ;
}	

void setter_max(double a) 
{
	if (a > 0)
	{
		maxe = a ;
		epsilon_zero = pow(10, -10) * maxe ;
	}
	else 
	{
		maxe = -a ;
		epsilon_zero = pow(10, -10) * maxe ;
	}
}
	 
double getter_max() 
{
	return maxe ;
}
	
double getter_epsilon_zero()
{
	return epsilon_zero ;
}

bool equal_zero(double b)
{
	if (abs(b)< epsilon_zero)
	{
		return true ;
	}
	else
	{
		return false ;
	}
}   

double normalise_c(double coordonnee)      
{
	if (coordonnee  > maxe)
	{
		do 
		{
			coordonnee = coordonnee - 2*maxe ;
		} while (coordonnee > maxe) ;
	}
	
	if (coordonnee  < -maxe )
	{
		do 
		{
			coordonnee = coordonnee + 2*maxe ;
		} while (coordonnee < -maxe) ;
	}

	return coordonnee ;
}

Point normalise_c(Point& p)
{
	p.x = normalise_c(p.x);
	p.y = normalise_c(p.y);

	return p ;
}

double Rebouclement::getter_v()
{
	return v ;
}

double Rebouclement::getter_vm()
{
	return vm ;
}

double Rebouclement::getter_vp()
{
	return vp ;
}

void Rebouclement::v_equivalent()
{
	vm = v - 2*maxe ;
	vp = v + 2*maxe ;
	
}	

void Rebouclement::setter_v(double a)
{
	if (v <= maxe and v >= -maxe)
	{
		v = a ;
	}
}	

double construction_vecteur(Vecteur a)
{
	return sqrt(a.x*a.x + a.y*a.y) ;
}
	
double construction_vecteur(Point a, Point b, Vecteur& z)
{
	double min_vecteur=sqrt(pow(((b.x+2*maxe)-(a.x)),2)+pow(((b.y+2*maxe)-(a.y)),2));
	
	for (int i(-1) ; i<=1 ; i++)
	{
		for (int j(-1) ; j<=1 ; j++)
		{
			double m=sqrt(pow(((b.x+2*i*maxe)-(a.x)),2)+pow(((b.y+2*j*maxe)-(a.y)),2));						
			
			if (m < min_vecteur)
			{
			   z.x = (b.x+2*i*maxe)-(a.x) ;
			   z.y = (b.y+2*j*maxe)-(a.y) ;
			   min_vecteur = m ;
		    }
		}
	}	
	return min_vecteur ;
}
	
double construction_vecteur(Point a, Point b)
{
	double min_vecteur = sqrt(((b.x+2*maxe)-(a.x))*((b.x+2*maxe)-(a.x)) + 
							((b.y+2*maxe)-(a.y))*((b.y+2*maxe)-(a.y))) ;
	
	for (int i(-1) ; i<=1 ; i++)
	{
		for (int j(-1) ; j<=1 ; j++)
		{
			double m = sqrt(((b.x+2*i*maxe)-(a.x))*((b.x+2*i*maxe)-(a.x)) + 
						((b.y+2*j*maxe)-(a.y))*((b.y+2*j*maxe)-(a.y))) ;
			
			if (m < min_vecteur)
			{
			    min_vecteur = m ;
		    }
		}
	}
		
	return min_vecteur ;
}	
		
bool egalite(Point a, Point b)
{
	double c = construction_vecteur(a, b) ;
	
	if (c < epsilon_zero)
	{
		return true ;
	}
	else
	{
		return false ;
	}
}

bool dans_le_cercle(Point a, Cercle b)
{
	double n = construction_vecteur(a, b.centre) ;
	
	if (n < b.rayon - epsilon_zero)
	{
		return true ;
	}
	else
	{
		return false ;
	}
}

bool inter_cercle(Cercle a, Cercle b)
{
	double n = construction_vecteur(a.centre, b.centre) ;
	
	if(n < a.rayon + b.rayon -epsilon_zero)
	{
		return true ;
	
	}
	else
	{
		return false ;
	}
}

void draw_black_disc(double x, double y, double r, double ressource)
{
	for (int i(-1) ; i<=1 ; i++)
	{
		for (int j(-1) ; j<=1 ; j++)
		{
			draw_gisement(x+2*i*maxe, y+2*j*maxe, r, ressource);
		}
	}
}

void draw_colored_circle(double x, double y,  int num_base)
{
	for (int i(-1) ; i<=1 ; i++)
	{
		for (int j(-1) ; j<=1 ; j++)
		{
			draw_base(x+2*i*maxe, y+2*j*maxe, num_base);
		}
	}
}

void draw_colored_target(double x, double y,  int num_base)
{
	draw_robot_prospection(x, y, num_base);
}

void draw_colored_wagon(double x, double y,  int num_base, bool b)
{
	draw_robot_transport(x, y, num_base, b);
}


void draw_colored_wifi_logo(double x, double y,  int num_base)
{
	draw_robot_communication(x, y, num_base);
}


void draw_colored_triangle(double x, double y,  int num_base)
{
	draw_robot_forage(x, y, num_base);
}


void draw_lign(Point a, Point b)
{
	Vecteur v;
	construction_vecteur(a, b ,v);
	draw_communication(a.x, a.y, a.x+v.x, a.y+v.y);	
}

void draw_grey_circle(double x, double y)
{
	for (int i(-1) ; i<=1 ; i++)
	{
		for (int j(-1) ; j<=1 ; j++)
		{
			draw_rayon_com(x+2*i*maxe, y+2*j*maxe);
		}
	}
}

