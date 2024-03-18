#ifndef POLY_GUARD
#define POLY_GUARD

#include <FL/fl_draw.H>
#include <FL/Fl_Image.H>
#include "Graph.h"
#include "Point.h"
#include "std_lib_facilities.h"

namespace Graph_lib
{
struct Poly : Polygon{
    //vector<Point> vec;
    double area;
    bool check();
};

struct Circles : Shape{
    vector<Point> ps;
    vector<int> rs;
    void add(Point p);
    void draw_lines() const;
};
}
#endif