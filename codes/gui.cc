#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib> 

#include "gui.h"
#include "graphic_gui.h"
#include "simulation.h"
#include "graphic.h"
#include "geomod.h"


#define GTK_COMPATIBILITY_MODE
#ifdef GTK_COMPATIBILITY_MODE
namespace Gtk
{
	template<class T, class... T_Args>
	auto make_managed(T_Args&&... args) -> T*
	{
		return manage(new T(std::forward<T_Args>(args)...));
	}
}
#endif

using namespace std;

namespace
{
	Simulation s;
	bool simulation_exist = false;
	bool echec = false ;
}

// =================================


MyArea::MyArea(): empty(false)
{
}

MyArea::~MyArea()
{
}

void MyArea::refresh()
{
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						 get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	double w_size = set_w_size(width, height);

	graphic_set_context(cr, w_size, width, height);
	cr->set_source_rgb(1,1,1);
	cr->paint();
	
	
	if(simulation_exist)
	{
		graphic_draw_frame();
		s.display_all();
	}
	return true;
}


GUI::GUI()
	:
	m_Box_all(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_Top(Gtk::ORIENTATION_HORIZONTAL, 10),
	m_Box_leftbutton(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_leftbutton_up(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_leftbutton_down(Gtk::ORIENTATION_VERTICAL, 10),
	m_Box_rightdraw(Gtk::ORIENTATION_HORIZONTAL, 10),
	m_Button_step("Step"),
	m_Button_exit("exit"),
	m_Button_open("open"),
	m_Button_save("save"),
	m_Button_start("start"),
	m_ToggleButton_toggle_link("toggle link"),
	m_ToggleButton_toggle_range("toggle range"),
	m_Frame_general("General"),
	m_Frame_toggle("Toggle"),
	started(false)
	 
{
	set_title("GUI");
	set_border_width(5);
	set_signal_handler();
	set_boxes();
	set_scroll_window();

	show_all_children();
  
}

GUI::~GUI()
{
}

void GUI::lecture_gui(char* argv)
{
	s.lecture(argv, echec);
	if(!echec) 
	{
		simulation_exist = true ;
	}
	Planet.refresh();
}

void GUI::on_button_clicked_step()
{
	if(started==false and simulation_exist)
	{
		s.infinie() ;
		tree_view_update();
		Planet.refresh() ;
		
	}
}
void GUI::on_button_clicked_exit()
{
	exit(EXIT_FAILURE);
}

void GUI::on_button_clicked_open()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
	Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	
	int result = dialog.run();
	
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::string filename = dialog.get_filename();
			s.lecture(filename, echec);
			if(!echec) 
			{
				simulation_exist = true;
			}
			Planet.refresh();

			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	} 
}

void GUI::on_button_clicked_save()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
	Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);
	
	int result = dialog.run();
	
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::string filename = dialog.get_filename();
			s.ecriture_simul(filename);
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	} 
}

void GUI::on_button_clicked_start()
{
	tree_view_update();
	state_simul();
}

void GUI::on_button_clicked_toggle_link()
{
	set_link_state();
}

void GUI::on_button_clicked_toggle_range()
{
	set_range_state();
}

bool GUI::on_key_press_event(GdkEventKey * key_event)
{
if(key_event->type == GDK_KEY_PRESS)
{
	switch(gdk_keyval_to_unicode(key_event->keyval))
	{
	case 's':
		tree_view_update();
		state_simul();
		Planet.refresh() ; 
		break;

	case '1':
		if(!started)
		{
		s.infinie() ;
		tree_view_update() ;
		Planet.refresh() ;
		}
		break;
	}
}
return Gtk::Window::on_key_press_event(key_event);
}



void GUI::state_simul()
{
	if(!started)
	{
		m_Button_start.set_label("stop");

		started = true;
	}else 
	{
		m_Button_start.set_label("start");
		started = false;
	}

}



void GUI::tree_view_update()
{
	Glib::RefPtr<Gtk::ListStore> ref_tree_model = Gtk::ListStore::create(_columns);
	_tree_view.set_model(ref_tree_model);
	

	if(simulation_exist) 
		{
			vector<SimData> report(s.get_base_info());
			for(size_t i = 0 ; i  <report.size() ; ++i)
			{
				auto row = *(ref_tree_model->append());
				row[_columns._col_nbP] = report[i].nbP;
				row[_columns._col_nbF] = report[i].nbF;
				row[_columns._col_nbT] = report[i].nbT;
				row[_columns._col_nbC] = report[i].nbC;
				row[_columns._col_resource] = report[i].ressource;
				if(report[i].ressource < 10000)
				{
				row[_columns._col_resource_percentage] = report[i].ressource_p;
				}else
				{
					row[_columns._col_resource_percentage] = 100;
				}
			}
		}
}

void GUI::set_link_state()
{
	toggle_link_state(m_ToggleButton_toggle_link.get_active());
	Planet.refresh();
}

void GUI::set_range_state()
{
	toggle_range_state( m_ToggleButton_toggle_range.get_active() );
	Planet.refresh();
}



bool GUI::on_idle()
{
	if(started and simulation_exist)
	{
		s.infinie() ;
		Planet.refresh() ;
		tree_view_update();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	return true;  
}

void GUI::set_signal_handler()
{
	m_Button_step.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_step));
	m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_exit) );

	m_Button_open.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_open) );
		  
	m_Button_save.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_save) );
		  
	m_Button_start.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_start) );
					  
	m_ToggleButton_toggle_link.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_toggle_link) );
		  
	m_ToggleButton_toggle_range.signal_clicked().connect(sigc::mem_fun(*this,
		  &GUI::on_button_clicked_toggle_range) );
		  
	Glib::signal_idle().connect( sigc::mem_fun(*this, &GUI::on_idle));
}

void GUI::set_boxes()
{	
	add(m_Box_all);
	
	m_Box_all.pack_start(m_Box_Top);
	m_Box_all.pack_start(m_Box_Bottom);
	
	m_Box_Bottom.pack_start(m_Box_leftbutton, Gtk::PACK_SHRINK);
	m_Box_Bottom.pack_start(m_Box_rightdraw);

	m_Frame_general.add(m_Box_leftbutton_up);
	m_Frame_toggle.add(m_Box_leftbutton_down);
	m_Box_leftbutton.pack_start(m_Frame_general, Gtk::PACK_SHRINK);
	m_Box_leftbutton.pack_start(m_Frame_toggle, Gtk::PACK_SHRINK);

	Planet.set_size_request(0.6*taille_dessin,0.6*taille_dessin);
	m_Box_rightdraw.pack_start(Planet);

	m_Box_leftbutton_up.pack_start(m_Button_exit, Gtk::PACK_SHRINK);
	m_Box_leftbutton_up.pack_start(m_Button_open, Gtk::PACK_SHRINK);
	m_Box_leftbutton_up.pack_start(m_Button_save, Gtk::PACK_SHRINK);
	m_Box_leftbutton_up.pack_start(m_Button_start, Gtk::PACK_SHRINK);
	m_Box_leftbutton_up.pack_start(m_Button_step, Gtk::PACK_SHRINK);

	m_Box_leftbutton_down.pack_start(m_ToggleButton_toggle_link, Gtk::PACK_SHRINK);
	m_Box_leftbutton_down.pack_start(m_ToggleButton_toggle_range, Gtk::PACK_SHRINK);
	
	m_Box_Top.add(_scrolled_window);
	
}

void GUI::set_scroll_window()
{	
	
	_scrolled_window.set_size_request(-1, 200); 
	_scrolled_window.add(_tree_view);

	_scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC,
						  Gtk::PolicyType::POLICY_AUTOMATIC);
	_scrolled_window.set_hexpand();

	_tree_view.append_column("nbP", _columns._col_nbP);
	_tree_view.append_column("nbF", _columns._col_nbF);
	_tree_view.append_column("nbT", _columns._col_nbT);
	_tree_view.append_column("nbC", _columns._col_nbC);
	_tree_view.append_column_numeric("Amount resource", _columns._col_resource,"%.2f");

	auto cell = Gtk::make_managed<Gtk::CellRendererProgress>();
	int cols_count = _tree_view.append_column("Mission completeness", *cell);

	auto progress_col = _tree_view.get_column(cols_count - 1);
	if(progress_col)
	progress_col->add_attribute(cell->property_value(),
							_columns._col_resource_percentage);
							
}
