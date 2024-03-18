#include <iostream>
#include "poly.h"
namespace Graph_lib {

inline pair<double, double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool &parallel);

bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point &intersection);

double crossProduct(const Point& a, const Point& b, const Point& c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
double dotProduct(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.x - a.x) + (b.y - a.y) * (c.y - a.y);
}

bool onSegment(const Point& p, const Point& q, const Point& r) {
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
           q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}
bool doSegmentsOverlap(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    double cross1 = crossProduct(p1, p2, p3);
    double cross2 = crossProduct(p1, p2, p4);
    if(cross1 != 0 || cross2 != 0){
        return false;
    }
    return onSegment(p1, p3, p2) || onSegment(p1, p4, p2) ||
           onSegment(p3, p1, p4) || onSegment(p3, p2, p4);
}

bool Poly::check() {
    int np = number_of_points();
    for (int i = 1; i < np - 2; i++) {
        Point ignore(0, 0);
        if (line_segment_intersect(point(0), point(np - 1), point(i), point(i + 1), ignore))
            return false;
    }
    bool isLeftTurn = crossProduct(point(0), point(1), point(2)) > 0;
    bool flag = false;
    for (int i = 1; i < np; ++i) {
        if ( (crossProduct(point(i), point((i + 1) % np), point((i + 2) % np)) > 0) != isLeftTurn ) {
            flag = true;
        }
    }
    if(!flag)
        return false;
    area = 0;
    for(int i = 0; i < np - 1; i++)
    {
        area += 0.5 * (point(i).x * point(i + 1).y - point(i).y * point(i + 1).x);
    }
    area += 0.5 * (point(np - 1).x * point(0).y - point(np - 1).y * point(0).x);

    int xmin = 1200, xmax = 0, ymin =700, ymax = 0;
    for(int i = 0; i < np; i++)
    {
        xmin = min(xmin, point(i).x);
        xmax = max(xmax, point(i).x);
        ymin = min(ymin, point(i).y);
        ymax = max(ymax, point(i).y);
    }
//    vector<Point> p_table;
//    for(int i = xmin; i <= xmax; i++)
//    {
//        for(int j = ymin; j <= ymax; j++)
//        {
//            Point inf_p = Point(i, 701);
//            int tmp = 0;
//            Point ignore(0, 0);
//            for(int k = 0; k < np; k++)
//            {
//                if(line_segment_intersect(Point(i, j),inf_p,point(k),point((k+1)%np),ignore))
//                    tmp++;
//            }
//            if(tmp % 2 != 0)
//                p_table.push_back(Point(i, j));
//            else
//            {
//                for(int k = 0; k < np; k++)
//                    if(Point(i,j) == point(k))
//                        p_table.push_back(Point(i, j));
//            }
//        }
//    }
    mx = (xmax-xmin)*(ymax-ymin);
    for(int i = xmin; i <= xmax; i++) {
        for(int j = ymin; j <= ymax; j++) {
            Point inf_p = Point(i, 701);
            int intersectionCount = 0;
            bool onVertexOrEdge = false;
            Point currentPoint(i, j);

            for(int k = 0; k < np; k++) {
                Point nextPoint = point((k + 1) % np);

                // 检查点是否是顶点
                if(currentPoint == point(k)) {
                    onVertexOrEdge = true;
                    p.push_back(currentPoint);
                    break;
                }
                Point ignore(0, 0);
                // 检查点是否在边上
                if(line_segment_intersect(currentPoint, inf_p, point(k), nextPoint, ignore)) {
                    if(ignore == currentPoint) { // 交点恰好是当前检查的点
                        onVertexOrEdge = true;
                        edge.push_back(currentPoint);
                        break;
                    }
                    intersectionCount++;
                }
            }

            // 如果点不在顶点或边上，且与多边形边的交点数为奇数，则点在多边形内部
            if(!onVertexOrEdge && (intersectionCount % 2 != 0)) {
                p_table.push_back(currentPoint);
            }
        }
    }

//    for(auto i : p_table)
//        std::cout << i.x <<"," <<i.y<<"  ";
    return true;
}
void Poly::calculate() {
    ans_area = 0.0;
    int h = 0;
    int np = number_of_points();
    for(int i = 0; i < p_table.size(); i++)
    {
        i = (i + p_table.size()/2) %  p_table.size();
        Point pn = p_table[i];
        vector<Point> list;
        for(int j = 0; j < np; j++)
        {
            Point pt = point(j);
            Point prv = point((j - 1 + np) % np);
            Point ne = point((j + 1) % np);
            double c1 = crossProduct(pt,pn,prv);
            double c2 = crossProduct(pt,pn,ne);
            bool flag = (c1 * c2) >= 0;

            if(!flag)
            {
                bool flag1 = true;
                for(int k = 0; k < np; k++)
                {
                    Point ignore(0, 0);
                    if(line_segment_intersect(pn,pt,point(k),point((k+1)%np),ignore))
                    {
                        if(ignore != pt)
                        {
                            flag1 = false;
                            break;
                        }
                    }
                }
                if(flag1)
                    list.push_back(pt);
            }
            else
            {
                bool flag1 = true;
                for(int k = 0; k < np; k++)
                {
                    Point ignore(0, 0);
                    if(line_segment_intersect(pn,pt,point(k),point((k+1)%np),ignore))
                    {
                        if(ignore != pt)
                        {
                            flag1 = false;
                            break;
                        }
                    }
                }
                if(flag1)
                {
                    double min = std::pow(1200,2);
                    Point pf = {pt.x + 2000 * (pt.x - pn.x), pt.y + 1000 * (pt.y - pn.y)};
                    Point ignore(0, 0);
                    for(int k = 0; k < np; k++)
                    {
                        Point tmp(0, 0);
                        if(line_segment_intersect(pt,pf,point(k),point((k+1)%np),tmp))
                        {
                            if(tmp != pt)
                            {
                                double dis = std::pow(ignore.x - pt.x,2);
                                if(dis < min)
                                {
                                    ignore = tmp;
                                    dis = min;
                                }
                            }
                        }
                    }
                    if(!list.size())
                    {
                        if(dotProduct(pt,point((i+1)%np),ignore)<0)
                        {
                            h=1;
                            list.push_back(pt);
                            list.push_back(ignore);
                        }
                        else
                        {
                            h=2;
                            list.push_back(ignore);
                            list.push_back(pt);
                        }
                    }
                    else
                    {
                        bool flag2 = false;
                        Point p_last = list[list.size()-1];
                        for(int k = 0; k < np; k++)
                        {
                            if(doSegmentsOverlap(p_last,pt,point(k),point((k+1)%np)))
                            {
                                flag2 = true;
                                list.push_back(pt);
                                list.push_back(ignore);
                                break;
                            }
                        }
                        if(!flag2)
                        {
                            if(list.size()!=2)
                            {
                                list.push_back(ignore);
                                list.push_back(pt);
                            }
                            else
                            {
                                h=3;
                                bool flag3 = false;
                                for(int k = 0; k < np; k++)
                                {
                                    if(doSegmentsOverlap(p_last,ignore,point(k),point((k+1)%np)))
                                    {
                                        flag3 = true;
                                        list.push_back(ignore);
                                        list.push_back(pt);
                                        break;
                                    }
                                }
                                if(!flag3)
                                {
                                    std::swap(list[0],list[1]);
                                    bool flag4 = false;
                                    p_last = list[1];
                                    for(int k = 0; k < np; k++)
                                    {
                                        if(doSegmentsOverlap(p_last,ignore,point(k),point((k+1)%np)))
                                        {
                                            flag4 = true;
                                            list.push_back(ignore);
                                            list.push_back(pt);
                                            break;
                                        }
                                    }
                                    if(!flag4)
                                    {
                                        list.push_back(pt);
                                        list.push_back(ignore);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        double tmpa = 0;
        for(int j = 0; j < list.size(); j++)
            tmpa += 0.5 * (list[j].x * list[(j+1)%list.size()].y - list[j].y * list[(j+1)%list.size()].x);
        tmpa = std::abs(tmpa);
        if(tmpa>area)
            continue;
//        if(tmpa==area)
//        {
//            ans_area = tmpa;
//            ans = pn;
//            ans_list.assign(list.begin(),list.end());
//            break;
//        }
        if(tmpa > ans_area)
        {
            ans_area = tmpa;
            ans = pn;
            ans_list.assign(list.begin(),list.end());
            std::cout<<pn.x<<" "<<pn.y<<" "<<h<<" "<<tmpa<<":";
            for(int m = 0; m < list.size();m++)
                std::cout << list[m].x <<" " << list[m].y <<"   ";
            std::cout<<"\n";
        }

    }
    std::cout<<area;
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