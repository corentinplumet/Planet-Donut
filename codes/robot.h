// Corentin Plumet 327547 Eliot Goncalves 330553
#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <memory>

#include "geomod.h"
#include "constantes.h"
#include "gisement.h"
#include "message.h"

class Robot
{
public :
	
	// constructeur
	Robot(unsigned int uid, double dp, Point position_courante,
		  Point position_but, bool atteint, Cercle sa_base, char nom, bool found);

	
	//calcul déplacement
	void calc_new_position(double d) ;		

	//getter
	char get_nom() {return nom;}
	bool get_atteint() {return atteint;}
	unsigned int get_uid() ;
	Point get_position_but() {return position_but ;}
	Point get_position_courante() ;
	double get_dp() {return dp ;} 
	void set_dp(double a); 	
	Cercle get_sa_base() {return sa_base;}
	
	//affichage
	virtual void display_robot(int m); 
	void display_com_radius();
	
	//connexion
	bool get_visited() const {return visited;}
	void set_visited(bool v);
	vector<shared_ptr<Robot>>const get_link() const {return link ;}
	void set_link(shared_ptr<Robot> r);
	
	//virtual
	virtual Gisement get_memoire() {Gisement a;
									return a;}	///pas très legit à vérifier///	
	virtual bool get_memory() {return false;} 		
	virtual void set_memory(bool b);		
	virtual void define_new_but() {}
	virtual void define_new_but(int compteur) {}		
	virtual double get_capaciteG() {return 0.0;}		
	virtual int get_arrivage() {return 0;}
	virtual void set_arrivage(int a);
		
	virtual void action_p(int compteur, vector<Gisement>& Eg) {}
	virtual void action_t(vector<shared_ptr<Robot>> r, 
						  vector<shared_ptr<Robot>> robots, double& ress, 
						  vector<Gisement>& Eg) {}
	virtual void action_f(vector<Gisement>& Eg) {}
	virtual void action_c(int nbc) {} ;
	virtual void action_t_autonomous(vector<shared_ptr<Robot>> robots, 
									 vector<Gisement>& Eg) {} ;
	virtual void action_c_autonomous(vector<Gisement>& Eg) {} ;
	virtual void action_f_autonomous(vector<Gisement>& Eg) {} ;
	virtual void action_p_autonomous(int compteur_angle_p, vector<Gisement>& Eg) {} ;
	virtual bool get_forage() {return false;}
	virtual void set_forage(bool a);
	virtual void set_retour(bool a);
	virtual void ajout_distance_atteint(double d) {}
	
	virtual string ecriture_robots() {return "";}
	
	// link
	void display_link();
	
	//destrcution
	void fonction_link() ;
	void efface_robot(unsigned int uidrobot, Point p) ;

protected :
		
	unsigned int uid ;
	double dp ;
	Point position_courante ;
	Point position_but ;
	bool atteint ;
	bool found ;
	char nom ;
	Cercle sa_base ;	
	
	//connexion
	vector<shared_ptr<Robot>> link ;
	bool visited ;
}; 


class Prospection : public Robot
{
public :	
	
	//constructeur
	Prospection(unsigned int uid, double dp, Point position_courante,
				Point position_but, bool atteint, bool retour, bool com, Cercle ba, 
				bool found, double d);
	
	//methode	
	void lecture_prosp() ;
	void action_p(int compteur_angle_p, vector<Gisement>& Eg) ;
	void reset_position_courante() ;
	void decision_retour_base() ;
	void define_new_but(int compteur_angle_p) ;
	Gisement get_memoire() ;
	bool get_memory() {return memory ;}
	void set_memory(bool b);
	void test_sur_gisement(vector<Gisement>& Eg) ;
	void display_robot(int m) ;
	void action_p_autonomous(int compteur_angle_p, vector<Gisement>& Eg) ;
	void set_retour(bool a) ;
	void ajout_distance_atteint(double d) {distance_atteint = distance_atteint + d ;}
	double get_direction() {return direction;}
	void tourniquet() ;
	
	string ecriture_robots();
				
private :
	
	bool retour ;
	bool com ;	
	Gisement memoire ;
	bool memory ;
	int compte ;
	int compteur_tourniquet ;
	int memoire_compteur_angle ;
	double distance_tourniquet ;
	double distance_atteint ;
	double direction ;
	bool m ;
};

class Transport : public Robot
{
public :
	
	//constructeur	
	Transport(unsigned int uid, double dp, Point position_courante, Point position_but,
			  bool atteint, bool retour, Cercle ba, bool found);
	
	//methode
	void test_but_transport() ;
	void fill_transport(size_t indice, vector<Gisement>& Eg) ;
	void empty_transport() ;
	void reset_position_courante() ;
	void decision_retour_base() ;
	void action_t(vector<shared_ptr<Robot>> r, vector<shared_ptr<Robot>> robots,
				  double& ress, vector<Gisement>& Eg) ;
	void define_new_but(vector<shared_ptr<Robot>> connectebase) ;
	void display_robot(int m);
	void action_t_autonomous(vector<shared_ptr<Robot>> robots, vector<Gisement>& Eg) ;
	void choix_forage(vector<shared_ptr<Robot>> connectebase, 
					  shared_ptr<Robot>& memoire_robot, int& k, int& j, bool& test);
	
	string ecriture_robots();
	
private :
	bool retour ;
	bool reservoir ;
	shared_ptr<Robot> forage_precedent ;
	int add ;
} ;

class Forage : public Robot
{
public :	
	
	//constructeur
	Forage(unsigned int uid, double dp, Point position_courante, Point position_but,
		   bool atteint, Cercle ba, bool found);
	
	//méthode
	void test_but_forage() ;
	void forer(Gisement gisement) ;
	void reset_position_courante() ;
	void action_f(vector<Gisement>& Eg) ;
	double get_capaciteG() {return capaciteG;} 
	int get_arrivage() {return arrivage;}
	void set_arrivage(int a);
	void actualise_memoire(vector<Gisement>& Eg) ;
	void display_robot(int m) ;
	void action_f_autonomous(vector<Gisement>& Eg) ;
	bool get_forage() {return forage ;}
	void set_forage(bool a);
	
	string ecriture_robots();
			
private :
	double capaciteG ;
	int arrivage ;		
	bool forage ;	
} ;

class Communication : public Robot
{
public:
		
	//constructeur
	Communication(unsigned int uid, double dp, Point position_courante, 
				  Point position_but, bool atteint, Cercle ba, bool found);
	
	//methode
	void test_but_com() ;
	void reset_position_courante() ;
	void decision_retour_base() ;
	void action_c(int nbc) ;
	void define_new_but(int nbc) ;
	void display_robot(int m) ;
	void action_c_autonomous() ;
	
	string ecriture_robots();
	
private:
} ;

Prospection decodage_prospection(istringstream& data, int& etat, int& k,
								 double& prospection, double& forage, 
								 double& transport, double& communication, Cercle ba, 
								 bool& echec, double z) ;

Forage decodage_forage(istringstream& data, int& etat, int& l, double& prospection,
					   double& forage, double& transport, 
					   double& communication, Cercle ba, bool& echec) ;

Transport decodage_transport(istringstream& data,int& etat, int& m,double& prospection,
							 double& forage, double& transport, double& communication,
							 Cercle ba, bool& echec) ;

Communication decodage_communication(istringstream& data, int& etat, int& n, 
									 double& prospection, double& forage, 
									 double& transport, double& communication,
									 Cercle ba, bool& echec) ;
									 
string convertb(bool b);

#endif // ROBOT_H_INCLUDED


