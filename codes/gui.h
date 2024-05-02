#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>

class MyArea : public Gtk::DrawingArea
{
public:

	MyArea();
	virtual ~MyArea();
	void refresh();
	
protected:
	
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:

	bool empty;
};

class GUI : public Gtk::Window
{

public:

	GUI();
	virtual ~GUI();
	void tree_view_update(); 
	void lecture_gui(char* argv);
	void display();
	
protected :

	bool on_idle();
	
private:
	
	Gtk::Box m_Box_all, m_Box_Top, m_Box_Bottom, m_Box_leftbutton, m_Box_leftbutton_up,
		 m_Box_leftbutton_down, m_Box_rightdraw;
	
	void on_button_clicked_step(); 
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	
	bool on_key_press_event(GdkEventKey * key_event);

	void on_button_clicked_toggle_link();
	void on_button_clicked_toggle_range();
	void state_simul();

	void set_link_state();

	void set_range_state();
	  

	MyArea		Planet;
	Gtk::Button		m_Button_step; 
	Gtk::Button		m_Button_exit;
	Gtk::Button		m_Button_open;
	Gtk::Button		m_Button_save;
	Gtk::Button		m_Button_start;
	Gtk::ToggleButton		m_ToggleButton_toggle_link;
	Gtk::ToggleButton		m_ToggleButton_toggle_range;

	Gtk::Frame 		  m_Frame_general;
	Gtk::Frame 		  m_Frame_toggle;
  
	void set_signal_handler();
	void set_boxes();
	void set_scroll_window();
	
	class Model_columns : public Gtk::TreeModel::ColumnRecord
	{
	public:
		Model_columns()
		{
		add(_col_id);
		add(_col_nbP);
		add(_col_nbF);
		add(_col_nbT);
		add(_col_nbC);
		add(_col_resource);
		add(_col_resource_percentage);
		}


		Gtk::TreeModelColumn<int> _col_id;
		Gtk::TreeModelColumn<int> _col_nbP;
		Gtk::TreeModelColumn<int> _col_nbF;
		Gtk::TreeModelColumn<int> _col_nbT;
		Gtk::TreeModelColumn<int> _col_nbC;
		Gtk::TreeModelColumn<double> _col_resource;
		Gtk::TreeModelColumn<int> _col_resource_percentage;
	};

	Model_columns _columns;

	Gtk::ScrolledWindow _scrolled_window;
	Gtk::TreeView _tree_view;

	bool started;
};

#endif // GTKMM_EXAMPLE_UID_H
