//gisement.cc Corentin Plumet 327547 Eliot Goncalves 330553  
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "gisement.h"
#include "geomod.h"
#include "message.h"

using namespace std;
using namespace message;

Cercle Gisement::get_positionG()
{ 
	return positionG ;
}

void nb_gisements(istringstream& data , int& etat, int& total, int& i, bool& echec)
{
	if(!(data >> total)) 
	{
		echec = true ;
	} 
	else 
	{
		i = 0 ;
	}
	if(total==0) 
	{
		etat=NB1 ;
	} 
	else
	{
		etat=GISEMENT ;
	} 
}

Gisement decodage_gisement(istringstream& data, int& etat, int& total, int& i,
						   bool& echec)
{	
	double xcentre ;
	double ycentre ;
	double rayon ;
	double ressources ;

	if(!(data >> xcentre >> ycentre >> rayon >> ressources )) 
	{
		echec = true ;
	}
	else 
	{
		++i ;
	}
	if(i == total) 
	{
		etat=NB1 ;
	} 
	Cercle c ;
	c.centre.x = xcentre ;
	c.centre.y = ycentre ;
	c.rayon = rayon ;
	Gisement g(c, ressources) ;
	
	return g ;
}


void Gisement::display_gisement()
{
	draw_black_disc(positionG.centre.x, positionG.centre.y, positionG.rayon, 
					gisementR);		
}

void vider_gisement(size_t indice, vector<Gisement>& Eg)
{
	Eg[indice].set_gisementR(Eg[indice].get_gisementR() - deltaR) ;
}

Gisement::Gisement(Cercle position, double r)
		  :positionG(position) , gisementR(r) {}
		  
void Gisement::set_gisementR(double R) 
{
	gisementR = R ;
}
