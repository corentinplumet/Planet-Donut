//base.h Corentin Plumet 327547 Eliot Goncalves 330553
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include "geomod.h"
#include "robot.h"
#include "gisement.h"

struct SimData
{
  unsigned nbP;
  unsigned nbF;
  unsigned nbT;
  unsigned nbC;
  double ressource;
  double ressource_p;
};


class Base
{
public :
	//constructeur
	Base(Cercle position , double r, int color, vector<shared_ptr<Robot>> robot={}) ;
	
	///methode///
	
	//getter-setter
	double get_ressource_base() {return ressource_base ;}
	void set_ressource_base(double x) ;
	Cercle get_position_base() ;
	vector<shared_ptr<Robot>> get_robots() 	{return robots;}
	vector<shared_ptr<Robot>> get_connecte_base() {return connecte_base ;}
	int get_nbp() {return nbp;}
	int get_nbf() {return nbf;}
	int get_nbt() {return nbt;}
	int get_nbc() {return nbc;}
	int get_c()	{return c;}
	
	void set_nbp(int a) ;
	void set_nbf(int a) ;
	void set_nbt(int a) ;
	void set_nbc(int a) ;
	
	//creation
	void creation() ; 
	void creation_p() ;
	void creation_f(shared_ptr<Robot> r) ;
	void creation_c() ;
	void creation_t(shared_ptr<Robot> r) ;
	
	//maintenance
	void maintenance() ;		
	
	//connexion
	void parcours_DFS_p() ;
	void connexion() ;
	
	//update-actualisation robot
	void update_robot(vector<Gisement>& Eg) ;
	void update_remote(shared_ptr<Robot> robot, vector<Gisement>& Eg) ;
	void update_autonomous(shared_ptr<Robot> robot, vector<Gisement>& Eg) ;
	
	//fonction générale
	void ajouter_robot(shared_ptr<Robot> r, bool& echec) ;
	void ajouter_robot(shared_ptr<Robot> nouveau) ;
	void detruire_robot() ;
	unsigned int testuid() ;
	
	//affichage
	void display_base();
	void afficher() ;
	
	//destruction
	void destruction() ;
	
	//Scrollbar
	SimData informations() ;
	
private :
	Cercle position_base ;
	double ressource_base ;
	vector<shared_ptr<Robot>> robots ;
	vector<shared_ptr<Robot>> connecte_base ;
	int nbp ;
	int nbf ;
	int nbt ;
	int nbc ;
	int compteur_angle_p ;
	int compteur_angle_c ;
	int c ;
} ;

void fill_tabB(Base baze, vector<Base>& Eb, vector<Gisement>, bool& echec) ;

void lecture_robot(istringstream& data, int& etat, int& k, int& l, int& m, int& n,
				   double& prospection, double& forage, double& transport,
				   double& communication, Base& baze, bool& echec) ;


void decodage_base(istringstream& data, int& etat, int& i, int& totalbase,
				   double& prospection, double& forage, double& transport,
				   double& communication, vector<Base>& Eb, vector<Gisement> Eg,
				   bool& echec );

void nb_bases(istringstream& data , int& etat, int& totalbase, int& i, bool& echec) ;

void rec_DFS_p(vector<shared_ptr<Robot>>& result, shared_ptr<Robot> next) ;

char nom_robot(shared_ptr<Robot> r) ;

void update_autonomous(shared_ptr<Robot> robot) ;

void do_link(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b, int k, int l) ;

void do_link_diff(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b, int k,
				  int l);

#endif // BASE_H_INCLUDED
