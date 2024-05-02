//~ graphic. Plumet Corentin 327547 Goncalves Eliot 330553
#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

void draw_base(double x, double y, int num_base);

void draw_gisement(double x, double y, double r, double ressource);

void draw_communication(double xa, double ya, double xb, double yb);

void draw_rayon_com(double x, double y);

void draw_robot_prospection(double x, double y,  int num_base);

void draw_robot_communication(double x, double y,  int num_base);

void draw_robot_forage(double x, double y,  int num_base);

void draw_robot_transport(double x, double y,  int num_base, bool b);


void choose_color(int num_base);

double set_w_size(double width, double height);

void graphic_draw_frame();

void toggle_link_state(bool b);

void toggle_range_state(bool c);


#endif
