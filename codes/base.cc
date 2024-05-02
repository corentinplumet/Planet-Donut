//base.cc Corentin Plumet 327547 Eliot Goncalves 330553
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "base.h"
#include "gisement.h"
#include "robot.h"
#include "geomod.h"
#include "constantes.h"
#include "message.h"

using namespace message ;

void decodage_base(istringstream& data, int& etat, int& i, int& totalbase,
				   double& prospection, double& forage, double& transport, 
				   double& communication, vector<Base>& Eb, vector<Gisement> Eg,
				   bool& echec)
{
	double xcentre ;
	double ycentre ;
	double ressources ;

	if(i != totalbase)
	{
		if(!(data >> xcentre >> ycentre >> ressources >> prospection >> forage >>
			 transport >> communication )) 
		{
			echec = true ;
		}
		else 
		{
			++i ;
		}	
	}	
	if(communication == 0)
	{
		cout << missing_robot_communication(xcentre, ycentre) ;
		echec = true ;
	}
	else 
	{
		etat=ROBOTBASE ;
	}		
	Cercle c ;
	c.centre.x = xcentre ;
	c.centre.y = ycentre ;
	c.rayon = rayon_base ;
	Base b(c, ressources, Eb.size()) ;
	fill_tabB(b, Eb, Eg, echec) ;
}

void nb_bases(istringstream& data , int& etat, int& totalbase, int& i, bool& echec)
{
	if(!(data >> totalbase)) 
	{
		exit(EXIT_FAILURE) ;
	} 
	else
	{
		i=0 ;
	}
	if(totalbase == 0) 
	{
		etat=FIN ;
	}
	else
	{
		etat=BASE ;
	} 
}


Cercle Base::get_position_base()
{ 
	return position_base ;
}

void lecture_robot(istringstream& data, int& etat, int& k, int& l, int& m, int& n,
				   double& prospection, double& forage, double& transport,
				   double& communication, Base& baze, bool& echec)
{

	if(prospection != k)
	{
		cout << "taille robots = " << baze.get_robots().size() << endl ;
		double z = baze.get_robots().size() + 1 ;
		baze.ajouter_robot(shared_ptr<Robot>(new Prospection(decodage_prospection(data, 
															 etat, k, prospection,
															 forage, transport,
															 communication,
															 baze.get_position_base(),
															 echec, z)))) ;
		baze.set_nbp(baze.get_nbp() + 1);
	}
	else if(forage != l)
	{
		baze.ajouter_robot(shared_ptr<Robot>(new Forage(decodage_forage(data, etat, l,
														prospection, forage, transport, 
														communication,
														baze.get_position_base(),
														echec)))) ;
		baze.set_nbf(baze.get_nbf() + 1);
	}
	else if(transport != m)
	{
		baze.ajouter_robot(shared_ptr<Robot>(new Transport(decodage_transport(data, 
														   etat, m, prospection,
														   forage, transport,
														   communication, 
														   baze.get_position_base(),
														   echec)))) ;
		baze.set_nbt(baze.get_nbt() + 1);															
	}
	else if(communication != n)
	{
		baze.ajouter_robot(shared_ptr<Robot>(new Communication(decodage_communication(
												 data, etat, n, prospection,forage,
												 transport,communication,
												 baze.get_position_base(), echec)))) ;
		baze.set_nbc(baze.get_nbc() + 1);
		if(communication == n)
		{
			bool b = false ;
			for(auto elem : baze.get_robots())
			{
				if(egalite((*elem).get_position_courante(),
							baze.get_position_base().centre)) 
				{
					b = true ;
				}
			}
			if(b == false) 
			{
				cout<< missing_robot_communication(baze.get_position_base().centre.x,
												   baze.get_position_base().centre.y);
				echec = true ;
			}		
		}				
	}
	else etat = BASE ;
}

void Base::ajouter_robot(shared_ptr<Robot> nouveau, bool& echec)
{
	if(nouveau != nullptr) 
	{
		for(auto elem : robots)
		{
			if((*elem).get_uid() == (*nouveau).get_uid())
			{
				cout << identical_robot_uid((*nouveau).get_uid()) ;
				echec = true ;
			}
		}
		robots.push_back(shared_ptr<Robot>(nouveau)) ;
	}
}

void Base::ajouter_robot(shared_ptr<Robot> nouveau)
{
	if(nouveau != nullptr) 
	{
		for(auto elem : robots)
		{
			if((*elem).get_uid() == (*nouveau).get_uid())
			{
				cout << identical_robot_uid((*nouveau).get_uid()) ;
			}
		}
		robots.push_back(shared_ptr<Robot>(nouveau)) ;
	}
}

void Base::detruire_robot()
{
	robots.clear() ;
}

void fill_tabB(Base baze, vector<Base>& Eb, vector<Gisement> Eg, bool& echec)
{
	for(auto elem : Eb)
	{
		if(inter_cercle(elem.get_position_base(), baze.get_position_base()))
		{
			Cercle e = elem.get_position_base() ;
			Cercle b = baze.get_position_base() ;
			cout << base_superposition(e.centre.x,e.centre.y,b.centre.x,b.centre.y);
			echec = true ;
		}
	}
	for(auto element : Eg)
	{
		if(inter_cercle(element.get_positionG(), baze.get_position_base()))
		{
			Cercle e = element.get_positionG() ;
			Cercle b = baze.get_position_base() ;
			cout << base_field_superposition(b.centre.x,  b.centre.y, 
											 e.centre.x, e.centre.y) ;
			echec =true ;
		}
	}
	Eb.push_back(baze) ;
}	


void Base::display_base()
{
	draw_colored_circle(position_base.centre.x, position_base.centre.y, c);
}

void Base::connexion()
{
	parcours_DFS_p() ;
}

////////////////////////			////////////////////////
//////////////////////// CONNEXION  ////////////////////////
////////////////////////			////////////////////////

void Base::parcours_DFS_p()
{
	bool test = true ;
	vector<shared_ptr<Robot>> result;
	
	for(auto p : robots) p->set_visited(false);
	
	for(unsigned int i(0) ; i < robots.size() ; ++i)
	{
		if(egalite(robots[i]->get_position_courante(), position_base.centre) and
				   robots[i]->get_nom() == 'c' and test == true)
		{
			rec_DFS_p(result, robots[i]);
			test = false ;			
		}
	}
	connecte_base = result ;
}


void rec_DFS_p(vector<shared_ptr<Robot>>& result, shared_ptr<Robot> next) //static ?
{
	next->set_visited(true) ;
	result.push_back(next) ;
	
	const vector<shared_ptr<Robot>> link = next->get_link();
	
	for(unsigned i(0) ; i < link.size() ; ++i)
	{
		if(! link[i]->get_visited())
		{
			rec_DFS_p(result, link[i]) ;
		}
	}
}

////////////////////////							////////////////////////
//////////////////////// UPDATE ROBOT,REMOTE,AUTO   ////////////////////////
////////////////////////							////////////////////////

void Base::update_robot(vector<Gisement>& Eg) 
{
	for(auto element : robots)
	{
		bool test = false ;
		shared_ptr<Robot> mem ;
		for(auto elem : connecte_base)
		{ 
			mem = elem ;
			if(element->get_uid() == elem->get_uid() and 
			   egalite(element->get_sa_base().centre, elem->get_sa_base().centre))
			{
				test = true ;
				update_remote(elem, Eg) ;
			}
		}
		if(test == false and egalite(element->get_sa_base().centre,
		   mem->get_sa_base().centre))
		{
			update_autonomous(element, Eg) ;
		}		
	}
	for(auto element : robots)
	{
		if(element->get_nom() == 'f')
		{
			element->set_forage(false) ;
		}
	}	
}


void Base::update_remote(shared_ptr<Robot> robot, vector<Gisement>& Eg) 
{
	switch(robot->get_nom())
	{
		case 'p' :
			robot->action_p(compteur_angle_p, Eg) ;
			break ;
		
		case 'f' :
			robot->action_f(Eg) ;
			break ;
			
		case 't' :
			robot->action_t(connecte_base, robots, ressource_base, Eg) ;
			break ;
		
		case 'c' :
			robot->action_c(compteur_angle_c) ;
			break ;
	}
}

void Base::update_autonomous(shared_ptr<Robot> robot, vector<Gisement>& Eg)
{
	switch(robot->get_nom())
	{
		case 'p' :
			robot->action_p_autonomous(compteur_angle_p, Eg) ;
			break ;
		
		case 'f' :
			robot->action_f(Eg) ;
			break ;
			
		case 't' :
			robot->action_t_autonomous(robots, Eg) ;
			break ;
		
		case 'c' :
			robot->action_c_autonomous(Eg) ;
			break ;
	}
}

////////////////////////				////////////////////////
//////////////////////// MAINTENANCE   	////////////////////////
////////////////////////				////////////////////////

void Base::maintenance()
{
	for(unsigned int i(0) ; i <robots.size() ; i++)
	{
		if(dans_le_cercle(robots[i]->get_position_courante() ,position_base))
		{
			if(robots[i]->get_dp() > 2*position_base.rayon)
			{			
				ressource_base = ressource_base-(cost_repair*robots[i]->get_dp());
				robots[i]->set_dp(0) ;
				if(robots[i]->get_nom() == 'p') 
				{
					robots[i]->set_retour(false) ;
					compteur_angle_p = compteur_angle_p + 1 ;
					robots[i]->define_new_but(compteur_angle_p) ;
				}	
			}
		}
	}
}

////////////////////////			////////////////////////
//////////////////////// CREATION   ////////////////////////
////////////////////////			////////////////////////

void Base::creation()			
{
	int x(0) ;
	for(int k(0); k<3; k++)
	{
		if(robots.size() < cases + 1)	
		{
			creation_c() ;
			x = x + 1 ;
		}
		else
		{
			for(auto elem : connecte_base)
			{
				if(elem->get_nom() == 'f' and elem->get_atteint() == true and
				   (elem->get_capaciteG() / (elem->get_arrivage()*deltaR)) > 5 and 
				   x < 3 and get_nbt() < 5)
				{
					elem->set_arrivage(elem->get_arrivage() + 1) ;
					creation_t(elem) ;
					x = x + 1 ;
				}
				if(elem->get_nom() == 'p' and elem->get_memory() == true and x != 3) 
				{
					creation_f(elem) ;
					x = x + 1 ;
				}
			}
		}
	}
	while(x<3 and get_nbp() < 12)
	{
		creation_p() ;
		x = x + 1 ;
	}	
}

unsigned int Base::testuid()
{
	bool uidtest;
	unsigned int uid(0) ;
	
	do
	{
		uidtest = true ;
		uid = uid+1 ;
		for(auto elem : robots)
		{
			if(uid == elem->get_uid()) 
			{
				uidtest = false ;
			}
		} 
	} while(uidtest == false) ;
	
	return uid ;
}

void Base::creation_c()
{
	if(ressource_base > cost_com)
	{
		unsigned int uid = testuid() ;
		ajouter_robot(shared_ptr<Robot>(shared_ptr<Robot>(new Communication(uid, 0,
															  position_base.centre, 
															  {0,0}, false, 
															  position_base,false))));
		set_nbc(get_nbc() + 1) ;	
		compteur_angle_c = compteur_angle_c + 1 ;
		robots[robots.size()-1]->define_new_but(compteur_angle_c) ;	
		ressource_base = ressource_base - cost_com ;	
	}
}

void Base::creation_t(shared_ptr<Robot> elem)
{
	if(ressource_base > cost_transp)
	{
		if((construction_vecteur(elem->get_position_courante(),position_base.centre)*2) 
			< maxD_transp)
		{
			unsigned int uid = testuid() ;
			ajouter_robot(shared_ptr<Robot>(shared_ptr<Robot>(new Transport(uid, 0,
														 position_base.centre,
														 elem->get_position_courante(),
														 false, false, position_base,
														 false)))) ;
			set_nbt(get_nbt() + 1) ;	
			ressource_base = ressource_base - cost_transp ;
		}
	}
}

void Base::creation_f(shared_ptr<Robot> elem)
{
	Gisement g(elem->get_memoire()) ;
	double d = construction_vecteur(g.get_positionG().centre, position_base.centre) ;
	bool test = false ;
	
	for(auto elem : connecte_base)
	{
		if(elem->get_nom() == 'f' and dans_le_cercle(elem->get_position_but(),
		   g.get_positionG())) test = true ;
	}
	
	if(g.get_gisementR() > cost_forage + cost_transp and
	   d < maxD_forage and test == false)
	{
		if(ressource_base > cost_forage)
		{
			Vecteur v ;
			double d = construction_vecteur(position_base.centre,
										  g.get_positionG().centre, v) ;
			double x = position_base.centre.x + v.x - 
					   (v.x*((g.get_positionG().rayon - deltaD)/d)) ;
			double y = position_base.centre.y + v.y - 
			           (v.y*((g.get_positionG().rayon - deltaD)/d)) ;
			Point p = {x, y} ;
			normalise_c(p) ;
			unsigned int uid = testuid() ;
			ajouter_robot(shared_ptr<Robot>(new Forage(uid, 0, position_base.centre,p, 
													   false, position_base, false)));
			set_nbf(get_nbf() + 1);	
			elem->set_memory(false) ;
			ressource_base = ressource_base - cost_forage ;
		}
	}
	else
	{
		elem->set_memory(false) ;
	}			
}

void Base::creation_p()
{
	if(ressource_base > cost_prosp)
	{
		unsigned int uid = testuid() ;
		ajouter_robot(shared_ptr<Robot>(shared_ptr<Robot>(new Prospection(uid, 0, 
															  position_base.centre, 
															  {0,0}, false, false, 
															  true, position_base,
															  false, 
															  robots.size()-51 )))) ;
		set_nbp(get_nbp() + 1);	
		compteur_angle_p = compteur_angle_p + 1 ;
		robots[robots.size()-1]->define_new_but(compteur_angle_p);	
		ressource_base = ressource_base - cost_prosp ;	
	}
}

void do_link(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b, int k, int l)
{
	bool present = false ;
	for(auto elem : a[k]->get_link())
	{
		if(elem->get_uid() == b[l]->get_uid())
		{
			present = true ;
		}
	}
	if(present == false) 
	{
		a[k]->set_link(b[l]) ;
	}
	
	present = false ;
	
	for(auto elem : b[l]->get_link())
	{
		if(elem->get_uid() == a[k]->get_uid()) 
		{
			present = true ;
		}
	}
	if(present == false)
	{
		b[l]->set_link(a[k]) ;
	}
}

void do_link_diff(vector<shared_ptr<Robot>> a, vector<shared_ptr<Robot>> b, int k,
				  int l)
{
	a[k]->set_link(b[l]) ;
	b[l]->set_link(a[k]) ;
}


void Base::destruction()
{
	for(auto elem : robots)
	{
		elem->fonction_link() ;
	}
	
	robots.clear() ;
}

SimData Base::informations()
{
	SimData sd;
	
	sd.nbP = nbp;
	sd.nbF = nbf;
	sd.nbT = nbt;
	sd.nbC = nbc;
	sd.ressource = ressource_base;
	sd.ressource_p = (ressource_base*100)/10000;

	return sd;
}

Base::Base(Cercle position , double r, int color, vector<shared_ptr<Robot>> robot)
	: position_base(position), ressource_base(r), robots(robot), nbp(0), nbf(0),
	  nbt(0), nbc(0), compteur_angle_p(0), compteur_angle_c(0), c(color) {}
	  
void Base::set_ressource_base(double x) 
{
	ressource_base = x ;
}

void Base::set_nbp(int a)
{
	nbp = a;
}

void Base::set_nbf(int a) 
{
	nbf = a;
}

void Base::set_nbt(int a) 
{
	nbt = a;
}

void Base::set_nbc(int a) 
{
	nbc = a;
}
