//~ graphic.cc Plumet Corentin 327547 Goncalves Eliot 330553
#include <cmath>
#include <iostream>
#include "graphic.h"
#include "graphic_gui.h"
#include "constantes.h"

constexpr double zero_rgb = 0.0;
constexpr double max_rgb = 1.0;
constexpr double half_rgb = 0.5;
constexpr double size_magnifier = 10.0;
constexpr double full_circle = 2 * M_PI;
constexpr double size_robot = 2.0;
constexpr double angle_origin = 0.0;
constexpr double line_width = 2.0;
constexpr double r_residual = 250;

static double window_size;
static double offsetX;
static double offsetY;
static bool link_state;
static bool range_state;
static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr) ;

double set_w_size(double width, double height)
{
if(width > height)
{return height;}
else
{return width;}
}

void toggle_link_state(bool b)
{
	link_state = b;
}

void toggle_range_state(bool c)
{
	range_state = c;
}

//conversion de coordonnées

double convertX(double X)
{
	double Xconv = (window_size * (X + dim_max )/(2*dim_max));
	return (Xconv+offsetX);
}

double convertY(double Y)
{
	double Yconv = (window_size * (dim_max-Y )/(2*dim_max));
	return (Yconv+offsetY);
}

void draw_base(double x, double y, int num_base)
{
	x =convertX(x);
	y =convertY(y);
	choose_color(num_base); 
	 
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->save();
	(*ptcr)->arc(x, y, size_magnifier*rayon_base, angle_origin, full_circle);   
	(*ptcr)->stroke();
}

void draw_gisement(double x, double y, double r, double ressource)
{
	double ressource_init =  1000.*(r/rayon_min)*(r/rayon_min); 
	double angle =( ((ressource-r_residual)*full_circle) / (ressource_init -r_residual) );
	
	x = convertX(x);
	y = convertY(y);
	r = (window_size*r)/(2*dim_max);
	(*ptcr)->set_source_rgb(zero_rgb, zero_rgb, zero_rgb); 
	(*ptcr)->set_line_width(line_width); 
	(*ptcr)->save();
	(*ptcr)->arc(x, y, r, angle_origin, full_circle);  
	(*ptcr)->fill_preserve();  
	(*ptcr)->stroke();
	(*ptcr)->set_source_rgb(max_rgb, max_rgb, max_rgb); 
	(*ptcr)->set_line_width(r/8); 
	(*ptcr)->arc(x, y, r/1.2, angle_origin, full_circle);  
	(*ptcr)->stroke();
	(*ptcr)->set_line_width(r/6);
	(*ptcr)->set_source_rgb(zero_rgb, zero_rgb, zero_rgb);
	(*ptcr)->arc(x, y, r/1.2, angle_origin, angle);  
	(*ptcr)->stroke();
	

}

void draw_communication(double xa, double ya, double xb, double yb)
{
	xa =convertX(xa);
	ya =convertY(ya);
	xb =convertX(xb);
	yb =convertY(yb);
	
	if(link_state)
	{
	(*ptcr)->set_source_rgb(half_rgb, zero_rgb, half_rgb);
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->move_to(xa, ya);
	(*ptcr)->line_to(xb, yb);
	(*ptcr)->stroke();
	}
}

void draw_rayon_com(double x, double y)
{
	x =convertX(x);
	y =convertY(y);
	if(range_state)
	{
		double r = (window_size*rayon_comm)/(2*dim_max);
		
		(*ptcr)->set_source_rgb(half_rgb, half_rgb, half_rgb); 
		(*ptcr)->set_line_width(line_width);
		(*ptcr)->save();
		for(int i=0; i<77; i+=4)
		{
			(*ptcr)->arc(x, y,r , i*M_PI/40, (i+2)*M_PI/40);    
			(*ptcr)->stroke();
		}
	
	}
}


void draw_robot_prospection(double x, double y,  int num_base)
{
	x =convertX(x);
	y =convertY(y);
	choose_color(num_base); 
	(*ptcr)->set_line_width(line_width/2);
	(*ptcr)->move_to(x-7.0, y);
	(*ptcr)->line_to(x+7.0, y);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x, y-7.5);
	(*ptcr)->line_to(x, y+7.5);
	(*ptcr)->stroke();
	(*ptcr)->arc(x, y, 1, 0, 2*M_PI);
	(*ptcr)->stroke();
	(*ptcr)->arc(x, y, 3, 0, 2*M_PI);
	(*ptcr)->stroke();
	(*ptcr)->arc(x, y, 5, 0, 2*M_PI);
	(*ptcr)->stroke();
}

void draw_robot_communication(double x, double y,  int num_base)
{
	x =convertX(x);
	y =convertY(y);
	choose_color(num_base);
	(*ptcr)->set_line_width(line_width); 
	(*ptcr)->arc(x, y, 0.5, 0, full_circle);
	(*ptcr)->stroke();
	(*ptcr)->arc(x, y, 3.5, M_PI +(M_PI/5) ,  -M_PI/5);
	(*ptcr)->stroke();
	(*ptcr)->arc(x, y, 7, M_PI +(M_PI/5) ,  -M_PI/5);
	(*ptcr)->stroke();
}

void draw_robot_forage(double x, double y,  int num_base)
{
	x =convertX(x);
	y =convertY(y);
	choose_color(num_base); 
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x+3, y-7.5);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x+3, y-7.5);
	(*ptcr)->line_to(x-3, y-7.5);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x-3, y-7.5);
	(*ptcr)->line_to(x, y);
	(*ptcr)->stroke();
}

void draw_robot_transport(double x, double y,  int num_base, bool b)
{
	x =convertX(x);
	y =convertY(y);

	choose_color(num_base); 
	(*ptcr)->set_line_width(line_width); 
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x+4.0, y);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x+4.0, y);
	(*ptcr)->line_to(x+4.0, y-4);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x-3.5, y);
	(*ptcr)->stroke();
	(*ptcr)->move_to(x-4.0, y);
	(*ptcr)->line_to(x-4.0, y-4);
	(*ptcr)->stroke();
	(*ptcr)->set_source_rgb(0.0, 0.0, 0.0); 
	(*ptcr)->arc(x+2.5, y+1.2, 0.75, 0, full_circle);
	(*ptcr)->stroke();
	(*ptcr)->arc(x-2.5, y+1.2, 0.75, 0, full_circle);
	(*ptcr)->stroke();
	if(b)
	{
	choose_color(num_base); 
	(*ptcr)->set_line_width(5.0); 
	(*ptcr)->move_to(x+0-4.0, y-3);
	(*ptcr)->line_to(x+4.6, y-3);
	(*ptcr)->stroke();
	(*ptcr)->set_line_width(2.0); 
		
	}
}

void choose_color(int num_base)
{
	switch((num_base+1)%6) 
	{
	case 1 :
	(*ptcr)->set_source_rgb(max_rgb, zero_rgb, zero_rgb); 
	// red 
	break;
	case 2 :
	(*ptcr)->set_source_rgb(zero_rgb, max_rgb, max_rgb); 
	// green 
	break;
	case 3 :
	(*ptcr)->set_source_rgb(zero_rgb, zero_rgb, max_rgb); 
	// blue 
	break;
	case 4 :
	(*ptcr)->set_source_rgb(max_rgb, max_rgb, zero_rgb); 
	// yellow 
	break;
	case 5 :
	(*ptcr)->set_source_rgb(max_rgb, zero_rgb, max_rgb); 
	// magenta 
	break ;
	case 6 :
	(*ptcr)->set_source_rgb(zero_rgb, max_rgb, max_rgb); 
	//cyan
	break;
			
	}
	
}

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr, double size, double width, double height)
{

	window_size = size;
	offsetX = (width-size)/2;
	offsetY = (height-size)/2;
	ptcr = &cr;
}

void graphic_draw_frame()
{
	(*ptcr)->set_source_rgb(half_rgb, half_rgb,half_rgb); 
	(*ptcr)->set_line_width(line_width);
	(*ptcr)->save();
	(*ptcr)->move_to(offsetX, offsetY);
	(*ptcr)->line_to(offsetX + window_size, offsetY);
	(*ptcr)->move_to(offsetX + window_size, offsetY);
	(*ptcr)->line_to(offsetX + window_size, offsetY + window_size);
	(*ptcr)->move_to(offsetX + window_size, offsetY + window_size);
	(*ptcr)->line_to(offsetX, offsetY + window_size);
	(*ptcr)->move_to(offsetX, offsetY + window_size);
	(*ptcr)->line_to(offsetX, offsetY);
	(*ptcr)->stroke();
}
