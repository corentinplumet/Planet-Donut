//simulation.cc Corentin Plumet 327547 Eliot Goncalves 330553 
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "simulation.h"
#include "base.h"
#include "gisement.h"
#include "geomod.h"
#include "robot.h"
#include "message.h"
#include "constantes.h"

using namespace std;
using namespace message ;

static int etat(0) ;
static int i(0), total(0), totalbase(0);
static double prospection, forage, transport, communication;
static string atteint, retour, com;

void Simulation::lecture(string nom_fichier, bool& echec)
{
    echec = false ;
    reset_static();
    string line;
    ifstream fichier(nom_fichier); 
    cout << success();
    if(!fichier.fail()) 
    {
        Eb.clear();
        Eg.clear();
        while(getline(fichier >> ws,line)) 
        {
            if(line[0]=='#')  continue;
            decodage_ligne(line, echec);

        }
        fichier.close() ;
    }
    else echec = true ;
}

void Simulation::decodage_ligne(string line, bool& echec)
{
	istringstream data(line);

	switch(etat) 
	{
		case NB0: 
		{
			nb_gisements(data, etat, total, i, echec) ;
			break;
		}	
		case GISEMENT:
		{
			fill_tabG(decodage_gisement(data, etat, total, i, echec), echec) ;
			break;
		}
		case NB1: 
			nb_bases(data, etat, totalbase, i, echec) ;
			break;
		case BASE: 
		{
			k = 0 ;
			l = 0 ;
			m = 0 ;
			n = 0 ;	
			decodage_base(data, etat, i, totalbase, prospection, 
						  forage, transport, communication, Eb, Eg, echec) ; 
			break;
		}
		case ROBOTBASE:
			lecture_robot(data, etat, k, l ,m ,n, prospection,
						  forage, transport, communication, Eb[Eb.size() - 1], echec);
			break ;						 
		case FIN: 
			break;
	}	
}					         

void Simulation::fill_tabG(Gisement gi, bool& echec)
{

	for(auto elem : Eg)
	{
		if(inter_cercle( elem.get_positionG(), gi.get_positionG() ) == true) 
		{
			Cercle e = elem.get_positionG() ;
			Cercle g = gi.get_positionG() ;
			cout << field_superposition(e.centre.x, e.centre.y, 
										g.centre.x, g.centre.y);
			echec = true ;
		}
	}
	Eg.push_back(gi) ;
}

void Simulation::display_all()
{
	for(auto n : Eg)
	{
		n.display_gisement();		
	}
	for(unsigned int k(0); k<Eb.size(); ++k)
	{
		Eb[k].display_base();
		double m = Eb[k].get_c();
		for(auto l : (Eb[k]).get_robots())
		{
			(*l).display_robot(m);
			(*l).display_com_radius();
			(*l).display_link();
		}
	}
}

void update_voisin(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b,int i,int j)
{
	for(unsigned int k(0); k<a.size(); k++)
	{
		for(unsigned int l(0); l<b.size(); l++)
		{
			if(i != j)
			{
				if(construction_vecteur(a[k]->get_position_courante(), 
				   b[l]->get_position_courante()) < rayon_comm)
				{
					do_link_diff(a, b, k, l) ;
				}
			}
			else
			{
				if(k != l)
				{
					if(construction_vecteur(a[k]->get_position_courante(), 
					   b[l]->get_position_courante()) < rayon_comm)
					{
						do_link(a, b, k, l) ;
					}
				}
			}
		}
	}
}


void Simulation::infinie()
{
	
	for(unsigned int i(0) ; i<Eb.size() ; i++)
	{
		for(unsigned int j(0) ; j <Eb.size() ; j++)
		{
			update_voisin(Eb[i].get_robots() ,Eb[j].get_robots(), i, j) ;
		}
				
		if(Eb[i].get_ressource_base() < 10000)
		{
			Eb[i].connexion() ;
			Eb[i].maintenance() ;
			Eb[i].creation() ;
			Eb[i].update_robot(Eg) ;
		}
		
	}
	
	for(unsigned int i(0) ; i < Eb.size() ; i++)
	{
		if(Eb[i].get_ressource_base() <= 0)
		{
			Eb[i].destruction() ;
			Eb.erase(Eb.begin() + i) ;
		}
	}
}					
					
					
vector<SimData> Simulation::get_base_info()
{
	vector<SimData> v;
	
	for(auto k : Eb)
	{
		v.push_back(k.informations());	
	}
	
	return v;
}				
					
void Simulation::ecriture_simul(string nom_fichier)
{
	string p, f, t, c;
	ofstream fichier(nom_fichier);
	
	fichier << "#"<< nom_fichier << endl << endl << "# gisements " << endl ;
	fichier << Eg.size() << endl;
	for(auto k : Eg)
	{
		fichier << k.get_positionG().centre.x << " " << k.get_positionG().centre.y 
		<< " " << k.get_positionG().rayon << " " << k.get_gisementR() << "\n";
	}
	fichier << endl << "# Bases " << endl;
	fichier << Eb.size() << " # nbB" << endl << endl;
	
	for(auto l : Eb)
	{
		fichier << l.get_position_base().centre.x << " " 
		<< l.get_position_base().centre.y << " " 
		<< l.get_ressource_base() 
		<< " " << l.get_nbp() 
		<< " " << l.get_nbf()  
		<< " " << l.get_nbt() 
		<< " " << l.get_nbc() << endl;
	
		for(auto k : (l.get_robots()) )
		{
			if(k->get_nom() == 'p') p+= k->ecriture_robots() ;	
			if(k->get_nom() == 'f')	f+= k->ecriture_robots();	
			if(k->get_nom() == 't')	t+= k->ecriture_robots() ;	
			if(k->get_nom() == 'c')	c+= k->ecriture_robots() ;
		}
		fichier << p << endl<< f << endl << endl<< t << endl<< endl << c ;	
	}
	fichier.close();
}						

void reset_static()
{    
	etat =0 ;
	i=0; 
	total = 0; 
	totalbase = 0;
	prospection = 0.0;
	forage = 0.0;
	transport = 0.0; 
	communication = 0.0;
	atteint = "";
	retour=""; 
	com = "";
}
