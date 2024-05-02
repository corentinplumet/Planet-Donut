//simulation.h Corentin Plumet 327547 Eliot Goncalves 330553 
#ifndef SIMULATION_H 
#define SIMULATION_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gisement.h"
#include "base.h"
#include "robot.h"


class Simulation
{
public :
	
	//constructeur
	Simulation()
	: Eb(vector<Base>{}) , Eg(vector<Gisement>{}) {}
		
	// traite le fichier ligne par ligne. 
		 
	void lecture(string nom_fichier, bool& echec) ;
		
	void fill_tabG(Gisement gi, bool& echec) ;
		
	void decodage_ligne(string line, bool& echec);	

	void display_all();
	
	void infinie() ;
	
	vector<SimData> get_base_info();
	
	void ecriture_simul(string nom_fichier);

private :
	vector<Base> Eb ;	
	vector<Gisement> Eg ;
	int k ;
	int l ;
	int m ;
	int n ;
} ;

void update_voisin(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b, 
				   int i, int j) ;

void reset_static() ;

#endif
