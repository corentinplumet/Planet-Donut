//projet.cc Corentin Plumet 327547 Eliot Goncalves 330553
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "gui.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "simulation.h"

using namespace std;

int main(int argc, char* argv[])
{		
	int const_argc(1);
	setter_max(dim_max) ;
	auto app = Gtk::Application::create(const_argc, argv,"Planet Donut");
	GUI gui;
		if(argc == 2) 
	{
		gui.lecture_gui(argv[1]) ;
	}

	return app->run(gui);
	
	
}

