#include "poly.h"
namespace Graph_lib {

inline pair<double, double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool &parallel);

bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point &intersection);


bool Poly::check() {
    int np = number_of_points();
    for (int i = 1; i < np - 2; i++) {
        Point ignore(0, 0);
        if (line_segment_intersect(point(0), point(np - 1), point(i), point(i + 1), ignore))
            return false;
    }
    area = 0;
    for(int i = 0; i < np - 1; i++)
    {
        area += 0.5 * (point(i).x * point(i + 1).y - point(i).y * point(i + 1).x);
    }
    area += 0.5 * (point(np - 1).x * point(0).y - point(np - 1).y * point(0).x);
    return true;
}

void Circles::add(Point p)
{
    Shape::add(Point(p.x - 2.5, p.y - 2.5));

}

void Circles::draw_lines() const
{
    int np = number_of_points();

    for(int i = 0; i < np; i++)
    {
        if (color().visibility())
            fl_arc(point(i).x,point(i).y,5,5,0,360);
    }
}
}