//gisement.h Corentin Plumet 327547 Eliot Goncalves 330553
#ifndef GISEMENT_H_INCLUDED
#define GISEMENT_H_INCLUDED

#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "constantes.h"
#include "geomod.h"

using namespace std ;

class Gisement 
{
public :
	// constructeur
	Gisement(Cercle position = {{0,0},1}, double r = 0) ;

	// methode
	Cercle get_positionG() ;
	void set_gisementR(double R) ;
	double get_gisementR() {return gisementR ;}
	void display_gisement();

private :
	Cercle positionG ;
	double gisementR ;	
} ;

void nb_gisements(istringstream& data , int& etat, int& total, int& i, bool& echec) ;

Gisement decodage_gisement(istringstream& data, int& etat, int& total, int& i,
						   bool& echec) ;

void vider_gisement(size_t indice, vector<Gisement>& Eg) ;

#endif // GISEMENT_H_INCLUDED

