//robot.cc Corentin Plumet 327547 Eliot Goncalves 330553
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include "robot.h"
#include "geomod.h"
#include "constantes.h"
#include "gisement.h"
#include "message.h"
using namespace std ;
using namespace message ;

constexpr int coef_dist = 500;

unsigned int Robot::get_uid()
{
	return uid ;
}

Point Robot::get_position_courante() 
{
	return position_courante ;
}

Prospection decodage_prospection(istringstream& data, int& etat, int& k,
								 double& prospection,double& forage,double& transport, 
								 double& communication,Cercle ba,bool& echec,double z)
{
	unsigned int uid ;
	double dp ;
	double xposition_courante, yposition_courante, xposition_but, yposition_but ;
	string atteint, retour, com ;

	if(k != prospection)
	{
		if (!(data >> uid >> dp >> xposition_courante >> yposition_courante >> 
			  xposition_but >> yposition_but >> atteint >> retour >> com))
		{
			echec = true ;
		}
		else ++k;
	}	
	if(k == prospection) 
	{
		if (forage != 0 or transport != 0 or communication != 0) etat = ROBOTBASE ;
		else etat = BASE;
	}
	bool a, r, c ;
	if(atteint == "true") a=true ;
	else                  a=false ;		
	if(retour == "true") r=true ;
	else                 r=false ; 	
	if(atteint == "true") c=true ;
	else                  c=false ;	
	Point pc ;
	pc.x = xposition_courante ;
	pc.y = yposition_courante ;
	Point pcb ;
	pcb.x = xposition_but ;
	pcb.y = yposition_but ;
	Prospection p(uid, dp, pc, pcb, a, r, c, ba, false, z*(2*M_PI/12)) ;
	cout << p.get_direction() << endl ;
	return p ;		
} 

Forage decodage_forage(istringstream& data, int& etat, int& l, double& prospection,
					   double& forage, double& transport, double& communication, 
					   Cercle ba, bool& echec)
{
		
	unsigned int uid ;
	double dp ;
	double xposition_courante, yposition_courante, xposition_but, yposition_but ;
	string atteint, retour, com ;
	
	if(l != forage)
	{
		if( !(data >> uid >> dp >> xposition_courante >> yposition_courante 
			>> xposition_but >> yposition_but >> atteint )) 
		{
			echec = true ;
		}
		else ++l;
	}
		
	if(l == forage) 
	{
		if(transport != 0 or communication != 0) etat = ROBOTBASE ;
		else etat = BASE ;
	}
	else etat=ROBOTBASE ;

	bool a;
	if(atteint == "true") a=true ;
	else                  a=false ;
	Point pc ;
	pc.x = xposition_courante ;
	pc.y = yposition_courante ;
	Point pcb ;
	pcb.x = xposition_but ;
	pcb.y = yposition_but ;
	Forage f(uid, dp, pc, pcb, a, ba, false) ;
	return f ;			
}

Transport decodage_transport(istringstream& data,int& etat,int& m,double& prospection,
							 double& forage, double& transport, double& communication,
							 Cercle ba, bool& echec)
{
	unsigned int uid ;
	double dp ;
	double xposition_courante, yposition_courante, xposition_but, yposition_but ;
	string atteint, retour, com ;
	if(m != transport)
	{
		if( !(data >> uid >> dp >> xposition_courante >> yposition_courante 
			>> xposition_but >> yposition_but >> atteint >> retour ))
		{
			echec = true ;
		}
		else ++m;
	}
		
	if(m == transport) 
	{
		if(communication != 0) etat = ROBOTBASE ;
		else etat = BASE ;
	}
	else etat=ROBOTBASE ;

	bool a, r;
	if(atteint == "true") a=true ;
	else                  a=false ;
	if(retour == "true") r=true ;
	else                 r=false ; 
	Point pc ;
	pc.x = xposition_courante ;
	pc.y = yposition_courante ;
	Point pcb ;
	pcb.x = xposition_but ;
	pcb.y = yposition_but ;
	Transport t(uid, dp, pc, pcb, a, r, ba, false) ;
	return t ; 
}

Communication decodage_communication(istringstream& data, int& etat, int& n, 
									 double& prospection, double& forage,
									 double& transport, double& communication,
									 Cercle ba, bool& echec)
{
	unsigned int uid ;
	double dp ;
	double xposition_courante ;
	double yposition_courante ;
	double xposition_but ;
	double yposition_but ;
	string atteint ;

	if(n != communication)
	{
		if( !(data >> uid >> dp >> xposition_courante >> yposition_courante >>
		      xposition_but	>> yposition_but >> atteint )) 
		{
			echec = true ;
		}
		else ++n;
	}
	if(n == communication) etat=BASE ;
	else etat=ROBOTBASE ;

	bool a;
	if(atteint == "true") a=true ;
	else                  a=false ;	 
	Point pc ;
	pc.x = xposition_courante ;
	pc.y = yposition_courante ;
	Point pcb ;
	pcb.x = xposition_but ;
	pcb.y = yposition_but ;
	Communication c(uid, dp, pc, pcb, a, ba, false) ;
	return c ;	 
}

////////////////////////			 ////////////////////////
//////////////////////// DEPLACEMENT ////////////////////////
////////////////////////			 ////////////////////////

void Robot::calc_new_position(double d)
{
	Vecteur z ;
	construction_vecteur(position_courante, position_but, z) ;
	double angle = atan2(z.y, z.x) ;
	position_courante.x = position_courante.x + d*cos(angle) ;
	position_courante.y = position_courante.y + d*sin(angle) ;
	dp = dp + d ;  
	if(nom == 'p') 
	{
		ajout_distance_atteint(d) ;
	}
}

////////////////////////					////////////////////////
//////////////////////// ACTION PROSPECTION ////////////////////////
////////////////////////					////////////////////////
void Prospection::action_p_autonomous(int compteur_angle_p, vector<Gisement>& Eg)
{
	if(dp != 0)
	{
		decision_retour_base() ;
		
		if(atteint == true) 
		{
			test_sur_gisement(Eg) ;
			position_but = sa_base.centre ;
		}
		else
		{
			reset_position_courante() ;
		}
	
		if(egalite(position_courante, position_but)) 
		{
			atteint = true ;
		}
	}
}

void Prospection::action_p(int compteur_angle_p, vector<Gisement>& Eg)
{
	decision_retour_base() ;
	if(atteint == true) 
	{
		if(retour == false)
		{
			test_sur_gisement(Eg) ;
			define_new_but(compteur_angle_p) ;
		}
	}
	else
	{
		reset_position_courante() ;
	}
	
	if(egalite(position_courante, position_but)) 
	{
		atteint = true ;
	}
}

void Prospection::test_sur_gisement(vector<Gisement>& Eg)
{
	for(auto elem : Eg)
	{
		if(dans_le_cercle(position_courante, elem.get_positionG()))
		{
			if(egalite(memoire.get_positionG().centre, {0,0}) and m == false)
			{
				memoire = elem ;
				memory = true ;
				m = true;
			}
			else 
			{
				if(egalite(memoire.get_positionG().centre,
				   elem.get_positionG().centre) == false)
				{
					memoire = elem ;
					memory = true ;
				}	
			}
		}
	}
}

void Prospection::reset_position_courante()
{
	if(atteint == false)
	{
		if(construction_vecteur(position_but, position_courante) >= deltaD)
		{ 
			calc_new_position(deltaD) ;
			normalise_c(position_courante) ;
			compte = compte + 1 ;
		}
		else
		{
			double d = construction_vecteur(position_but, position_courante) ;
			calc_new_position(d) ;
			normalise_c(position_courante) ;
			compte = compte + 1;
		}
	}
}
	
void Prospection::define_new_but(int compteur_angle_p)
{	
	if(dp == 0)
	{
		distance_tourniquet = 50*deltaD ;
		direction = compteur_angle_p*(2*M_PI/12) ; 
		memoire_compteur_angle = compteur_angle_p - 1 ;
		position_but.x = position_courante.x + distance_tourniquet * cos(direction) ;
		position_but.y = position_courante.y + distance_tourniquet * sin(direction) ;
		atteint = false ;
		found = false ;
	}
	else
	{
		tourniquet() ;
	}
}

void Prospection::tourniquet()
{
	if(distance_tourniquet <= distance_atteint)
	{
		distance_atteint = 0;
		compteur_tourniquet = compteur_tourniquet + 1 ;
		if(compteur_tourniquet%2 == 0)
		{
			direction = direction + M_PI/2 ;
			if(dp > deltaD * coef_dist) distance_tourniquet = 0.5*dp ;
			else distance_tourniquet = 0.2*dp ;
		}
		if(compteur_tourniquet%2 == 1)
		{
			direction = direction - M_PI/2 ;
			if(dp > deltaD * coef_dist) distance_tourniquet = 0.5*dp ;
			else distance_tourniquet = 0.2*dp ;
		}
		position_but.x = position_courante.x + rayon_min * cos(direction) ;
		position_but.y = position_courante.y + rayon_min * sin(direction) ;
		normalise_c(position_but) ;
		atteint = false ;
	}
	else
	{
		position_but.x = position_courante.x + rayon_min * cos(direction) ;
		position_but.y = position_courante.y + rayon_min * sin(direction) ;
		normalise_c(position_but) ;
		atteint = false ;
	}
}

void Prospection::decision_retour_base()
{
	if((maxD_prosp - dp) - construction_vecteur(position_courante, sa_base.centre) <
	   2*deltaD)
	{
		atteint = false ;
		position_but = sa_base.centre ;
		retour = true ;
	}
}

Gisement Prospection::get_memoire()
{
	return memoire ;
}

////////////////////////				////////////////////////
//////////////////////// ACTION FORAGE  ////////////////////////
////////////////////////				////////////////////////

void Forage::action_f_autonomous(vector<Gisement>& Eg)
{
	if(dp != 0)
	{
		if(atteint == false) 
		{
			reset_position_courante() ;
			test_but_forage() ;
		}
		else
		{
			actualise_memoire(Eg) ;
		}
	}
}

void Forage::action_f(vector<Gisement>& Eg)
{
	if(atteint == false) 
	{
		reset_position_courante() ;
		test_but_forage() ;
	}
	if(atteint == true) 
	{
		actualise_memoire(Eg) ;
	}
}

void Forage::actualise_memoire(vector<Gisement>& Eg)
{
	for(auto elem : Eg)
	{
		if(dans_le_cercle(position_courante, elem.get_positionG()))
		{
			capaciteG = elem.get_gisementR() ; 
		}
	}
}
	
void Forage::test_but_forage()
{
	if(egalite(position_but, position_courante))
	{
		atteint = true ; 
	}
}

void Forage::reset_position_courante()
{
	if(atteint == false)
	{
		if(construction_vecteur(position_but, position_courante) >= deltaD)
		{ 
			calc_new_position(deltaD) ;
			normalise_c(position_courante) ;
		}
		else
		{
			double d = construction_vecteur(position_but, position_courante) ;
			calc_new_position(d) ;
			normalise_c(position_courante) ;
		}
	}
}

////////////////////////					////////////////////////
//////////////////////// ACTION TRANSPORT   ////////////////////////
////////////////////////					////////////////////////
void Transport::action_t_autonomous(vector<shared_ptr<Robot>> robots,
									vector<Gisement>& Eg)
{	
	if(dp != 0)
	{
		if(atteint == true and retour == false)
		{
			for(auto elem : robots)
			{
				if(elem->get_nom() == 'f' and 
				   egalite(position_courante, elem->get_position_courante()) and
				   reservoir == false)
				{
					for(unsigned int f(0); f<Eg.size(); f++)
					{
						if(dans_le_cercle(position_courante, Eg[f].get_positionG()))
						{
							if(Eg[f].get_gisementR() > deltaR)
							{
								fill_transport(f, Eg) ;
								elem->set_arrivage(elem->get_arrivage() -1) ;
							}	
						}
					}
				}
			}
			position_but = sa_base.centre ;		
		}
		else 
		{
			reset_position_courante() ;
		}
		
		test_but_transport() ;
		decision_retour_base() ;
	}
}

void Transport::action_t(vector<shared_ptr<Robot>> connectebase, 
						 vector<shared_ptr<Robot>> robots, double& ress,
						 vector<Gisement>& Eg)
{	
	if(atteint == true)
	{
		if(retour == true)
		{
			if(reservoir == true)
			{
				ress = ress + deltaR ;
				reservoir = false ;
				define_new_but(connectebase) ;
			} 
			else define_new_but(connectebase) ;
		}
		else
		{
			bool memoire ;
			for(auto elem : robots)
			{
				if(elem->get_nom() == 'f' and 
				   egalite(position_courante, elem->get_position_courante()) and 
				   reservoir == false)
				{
					for(unsigned int f(0); f<Eg.size(); f++)
					{
						if(dans_le_cercle(position_courante, Eg[f].get_positionG()))
						{
							if(Eg[f].get_gisementR() > deltaR)
							{
								if(elem->get_forage() == false)
								{
									memoire = elem->get_forage() ;
									fill_transport(f, Eg) ;
									elem->set_arrivage(elem->get_arrivage() -1) ;
									elem->set_forage(true) ;
								}
							}	
						} 
					}
				}
			}
			if(memoire == false) define_new_but(connectebase) ;
		}
	}
	else 
	{
		reset_position_courante() ;
		test_but_transport() ;
	}
	decision_retour_base() ;
}
	
void Transport::test_but_transport()
{
	if(egalite(position_courante, position_but))
	{ 
		atteint = true ;
	}
}

void Transport::define_new_but(vector<shared_ptr<Robot>> connectebase)
{
	if(atteint == true and retour == true and reservoir == false)
	{
		bool test(false);
		int k(0) ;
		int j ;
		shared_ptr<Robot> memoire_robot ;		
		choix_forage(connectebase, memoire_robot, k, j, test) ;
		if(test == true)
		{
			position_but.x = connectebase[j]->get_position_courante().x ;
			position_but.y = connectebase[j]->get_position_courante().y ;
			normalise_c(position_but) ;
			atteint = false ;
			retour = false ;
			memoire_robot->set_arrivage(memoire_robot->get_arrivage() +1) ;
			
		}
	}
	if(atteint == true and retour == false)
	{
		retour = true ;
		atteint = false ;
		position_but.x = sa_base.centre.x ;
		position_but.y = sa_base.centre.y ;
	}
}

void Transport::choix_forage(vector<shared_ptr<Robot>> connectebase, 
							 shared_ptr<Robot>& memoire_robot, int& k, int& j,
							 bool& test)
{
	for(unsigned int i(0) ; i < connectebase.size() ; i++)
	{
		double d;
		if(connectebase[i]->get_nom() == 'f' and 
		   connectebase[i]->get_atteint() == true and 
		   (connectebase[i]->get_capaciteG() - 
			connectebase[i]->get_arrivage()*deltaR) > deltaR)
		{
			if(k == 0)
			{
				d = construction_vecteur(connectebase[i]->get_position_courante(),
										 position_courante) ;
				j = i ;
				test = true ;
				memoire_robot = connectebase[i] ;
			}
			else
			{
				double dd = construction_vecteur(
							connectebase[i]->get_position_courante(),
							position_courante) ;
				if(dd < d)
				{
					d = dd ;
					j = i ;
					test = true ;
					memoire_robot = connectebase[i] ;
				}
				k = k + 1 ;
			}
		}	
	}
}

void Transport::reset_position_courante()
{
	if(atteint == false)
	{
		if(construction_vecteur(position_but, position_courante) >= deltaD)
		{ 
			calc_new_position(deltaD) ;
			normalise_c(position_courante) ;
		}
		else
		{
			double d = construction_vecteur(position_but, position_courante) ;
			calc_new_position(d) ;
			normalise_c(position_courante) ;
		}
	}
}

void Transport::decision_retour_base()
{
	if((maxD_transp - dp) - 
	   construction_vecteur(position_courante, sa_base.centre) < 2*deltaD )
	{
		atteint = false ;
		position_but = sa_base.centre ;
		retour = true ;
	}
}

void Transport::fill_transport(size_t indice, vector<Gisement>& Eg)
{
	reservoir = true ;
	vider_gisement(indice, Eg) ;
}

////////////////////////						////////////////////////
//////////////////////// ACTION COMMUNICATION   ////////////////////////
////////////////////////						////////////////////////
void Communication::action_c_autonomous()
{
	if(dp !=0)
	{
		if(atteint == false)
		{
			reset_position_courante() ;
			normalise_c(position_courante) ;
		}
		test_but_com() ;
	}
}

void Communication::action_c(int compteur_angle_c)
{
	if(atteint == false)
	{
		reset_position_courante() ;
		normalise_c(position_courante) ;
	}
	test_but_com() ;
}


void Communication::define_new_but(int nbc)
{
    position_but.x = (nbc % dg)*((2*dim_max)/dg);
    position_but.y = (nbc / dg)*((2*dim_max)/dg);
}
		
	
void Communication::test_but_com()
{
	if(egalite(position_courante, position_but))
	{
		atteint = true ;
	}
}

void Communication::reset_position_courante()
{
	if(atteint == false and dp < maxD_com)
	{
		if(construction_vecteur(position_but, position_courante) >= deltaD)
		{ 
			calc_new_position(deltaD) ;
		}
		else
		{
			double d = construction_vecteur(position_but, position_courante) ;
			calc_new_position(d) ;
		}
	}
}

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

void Robot::display_robot(int m)
{	
	//~ draw_colored_dot(position_courante.x, position_courante.y, m);
}

void Prospection::display_robot(int m)
{	
	draw_colored_target(position_courante.x, position_courante.y, m);
}

void Transport::display_robot(int m) 
{	
	draw_colored_wagon(position_courante.x, position_courante.y, m, reservoir);
}

void Communication::display_robot(int m)
{	
	draw_colored_wifi_logo(position_courante.x, position_courante.y, m);
}

void Forage::display_robot(int m)
{
	draw_colored_triangle(position_courante.x, position_courante.y, m);
}

void Robot::display_com_radius()
{
	draw_grey_circle(position_courante.x, position_courante.y);
}

void Robot::display_link()
{
    for(auto l : link)
    {
		if(construction_vecteur(position_courante, (*l).get_position_courante()) <= 
		   rayon_comm )
		{
			draw_lign(position_courante, (*l).get_position_courante() );
		}
    }
}


void Robot::fonction_link()
{
	for(auto elem : link)
	{
		elem->efface_robot(uid, sa_base.centre) ;
	}
}
	
void Robot::efface_robot(unsigned int uidrobot, Point p)
{
	for(unsigned int i(0) ; i<link.size() ; i++)
	{
		if(link[i]->get_uid() == uidrobot and 
		   egalite(p, link[i]->get_sa_base().centre))
		{
			link.erase(link.begin() + i) ;
		}
	}
}

//////////////////// ECRITURE ///////////////////:

string Prospection::ecriture_robots()
{
	string s;
	s+= string("   ") + to_string(uid) + string(" ") + to_string(dp) + string(" ") 
				+ to_string(position_courante.x) + string(" ") 
				+ to_string(position_courante.y) + string(" ") 
				+ to_string(position_but.x) + string(" ") + to_string(position_but.y) 
				+ string(" ") + convertb(atteint) + string(" ") + convertb(retour) 
				+ string(" ") + convertb(found) + string(" ") 
				+ to_string(memoire.get_positionG().centre.x) + string(" ") 
				+ to_string(memoire.get_positionG().centre.y) + string(" ") 
				+ to_string(memoire.get_gisementR()) + string("\n") ;
	return s;
}

string Transport::ecriture_robots()
{
	string s;
	s+= string("   ") + to_string(uid) + string(" ") + to_string(dp) + string(" ") 
				+ to_string(position_courante.x) + string(" ") 
				+ to_string(position_courante.y) + string(" ") 
				+ to_string(position_but.x) + string(" ") + to_string(position_but.y) 
				+ string(" ") + convertb(atteint) + string(" ") + convertb(retour)
				+ string("\n") ;
	return s;
}

string Forage::ecriture_robots()
{
	string s;
	s+= string("   ") + to_string(uid) + string(" ") + to_string(dp) + string(" ") 
				+ to_string(position_courante.x) + string(" ")
				+ to_string(position_courante.y) + string(" ") 
				+ to_string(position_but.x) + string(" ") + to_string(position_but.y) 
				+ string(" ") + convertb(atteint) + string(" ") + string("\n") ;
	return s;
}

string Communication::ecriture_robots()
{
	string s;
	s+= string("   ") + to_string(uid) + string(" ") + to_string(dp) + string(" ") 
				+ to_string(position_courante.x) + string(" ") 
				+ to_string(position_courante.y) + string(" ") 
				+ to_string(position_but.x) + string(" ") + to_string(position_but.y) 
				+ string(" ") + convertb(atteint) + string("\n") ;
	return s;
}

string convertb(bool b)
{
	if(b==true)
	{
		return "true";
	}
	else 
	{
		return "false";
	}
}

Robot::Robot(unsigned int uid, double dp, Point position_courante,
			 Point position_but, bool atteint, Cercle sa_base, char nom, bool found)
: uid(uid), dp(0), position_courante(position_courante),
position_but(position_but), atteint(atteint), found(found), nom(nom),
sa_base(sa_base), visited(false)
{} ;

void Robot::set_dp(double a) {dp = a;} ;

void Robot::set_visited(bool v) {visited = v;}

void Robot::set_link(shared_ptr<Robot> r) {link.push_back(r) ;}

void Robot::set_memory(bool b) {}	

void Robot::set_arrivage(int a) {}

void Robot::set_forage(bool a) {}

void Robot::set_retour(bool a) {}	

Prospection::Prospection(unsigned int uid, double dp, Point position_courante,
			Point position_but, bool atteint, bool retour, bool com, Cercle ba, 
			bool found, double d)
: Robot(uid, dp, position_courante, position_but, atteint, ba, 'p', found), 
retour(retour), com(com), memory(false), compte(0), 
compteur_tourniquet(0), memoire_compteur_angle(0),
distance_tourniquet(0), distance_atteint(0), direction(d)
{}

void Prospection::set_memory(bool b) {memory = b ;}

void Prospection::set_retour(bool a) {retour = a;}

Transport::Transport(unsigned int uid, double dp, Point position_courante, Point position_but,
		  bool atteint, bool retour, Cercle ba, bool found)
: Robot(uid, dp, position_courante, position_but, atteint, ba, 't', found), 
		retour(retour),  reservoir(false)
{}

Forage::Forage(unsigned int uid, double dp, Point position_courante, Point position_but,
	   bool atteint, Cercle ba, bool found)
: Robot(uid, dp, position_courante, position_but, atteint, ba, 'f', found), 
		arrivage(0), forage(false)
{}

void Forage::set_arrivage(int a) {arrivage = a;}

void Forage::set_forage(bool a) {forage = a ;}

Communication::Communication(unsigned int uid, double dp, Point position_courante, 
			  Point position_but, bool atteint, Cercle ba, bool found)
: Robot(uid, dp, position_courante, position_but, atteint, ba, 'c', found)
{}

