#ifndef My_WINDOW_GUARD
#define My_WINDOW_GUARD

#include "GUI.h"
#include "Graph.h"
#include "poly.h"
#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace Graph_lib;


struct My_window : Graph_lib::Window {
    My_window(Point xy, int w, int h, const string& title);
    bool wait_for_button(); // simple event loop

private:

    Poly area;
    Circles circles;
    Lines lines;
    double rr;

    In_box address;
    In_box ratio;
    Button nxt_button;

    In_box next_x;
    In_box next_y;

    Button add_button;

    Button next_button;     // the "next" button
    bool next_pushed;     // implementation detail

    Button quit_button;
    bool quit_pushed;

    Out_box s_pixel;
    Out_box s_true;

    Vector_ref<Image> ii;

    static void cb_add(Address, Address);
    void addi();

    static void cb_next(Address, Address); // callback for next_button
    void next();            // action to be done when next_button is pressed

    static void cb_quit(Address, Address);
    void quit();

    static void cb_nxt(Address, Address);
    void nxt();

};



#endif
