#ifndef POLY_GUARD
#define POLY_GUARD

#include <FL/fl_draw.H>
#include <FL/Fl_Image.H>
#include "Graph.h"
#include "Point.h"
#include "std_lib_facilities.h"
#include <cmath>

namespace Graph_lib
{
struct Poly : Polygon{
    //vector<Point> vec;
    double area;
    bool check();
    void calculate();
    Point ans;
    double ans_area;
    vector<Point> p;
    vector<Point> edge;
    vector<Point> p_table;
    vector<Point> ans_list;
    double mx;
};

struct Circles : Shape{
    vector<Point> ps;
    vector<int> rs;
    void add(Point p);
    void draw_lines() const;
};
}
#endif