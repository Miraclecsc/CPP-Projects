#include "My_window.h"

std::string format_scientific(double number, int precision) {
    if (number == 0) {
        return "0";
    }

    // 计算指数部分
    int exponent = static_cast<int>(std::floor(std::log10(std::abs(number))));

    // 计算系数部分
    double coefficient = number / std::pow(10.0, exponent);

    // 使用 stringstream 进行格式化
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << coefficient << " * 10^" << exponent;

    return ss.str();
}
std::string pointToString(const Point& p) {
    std::ostringstream oss;
    oss << "(" << p.x << ", " << p.y - 40 << ")";
    return oss.str();
}

inline pair<double,double> line_intersect_w(Point p1, Point p2, Point p3, Point p4, bool& parallel)
{
    double x1 = p1.x;
    double x2 = p2.x;
    double x3 = p3.x;
    double x4 = p4.x;
    double y1 = p1.y;
    double y2 = p2.y;
    double y3 = p3.y;
    double y4 = p4.y;

    double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
    if (denom == 0){
        parallel= true;
        return pair<double,double>(0,0);
    }
    parallel = false;
    return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
                                ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}

//------------------------------------------------------------------------------
bool line_segment_intersect_w(Point p1, Point p2, Point p3, Point p4, Point& intersection){
    bool parallel;
    pair<double,double> u = line_intersect_w(p1,p2,p3,p4,parallel);
    if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
    intersection.x = p1.x + u.first*(p2.x - p1.x);
    intersection.y = p1.y + u.first*(p2.y - p1.y);
    return true;
}

//inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel);
//bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection);

bool cmp(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first < p2.first;
}

My_window::My_window(Point xy, int w, int h, const string& title) :
        Window(xy,w,h,title),
        nxt_button(Point(x_max() / 2 - 100, 0), 200, 20, "Start: ",cb_nxt),
        csc(Point(x_max() / 2 - 100, 0), 200, 20, "常烁晨_Project2", cb_csc),
        add_button(Point(x_max()-300, 20),100, 20, "Add", cb_add),
        next_button(Point(x_max()-200,20), 100, 20, "Draw", cb_next),
        next_pushed(false),
        quit_button(Point(x_max()-100,20), 100, 20, "Quit", cb_quit),
        quit_pushed(false),
        s_pixel(Point(475, 20), 150, 20, "摄像机位置(x, y)："),
        s_true(Point(725, 20), 150, 20, "覆盖面积："),
        next_x(Point(75, 20), 100, 20, "输入 x："),
        next_y(Point(250, 20), 100, 20, "输入 y：")
{
    attach(nxt_button);
}

//------------------------------------------------------------------------------

bool My_window::wait_for_button()
{
    show();
    quit_pushed = false;
#if 1
    // Simpler handler
    while (!quit_pushed) Fl::wait();
    Fl::redraw();
#else
    // To handle the case where the user presses the X button in the window frame
    // to kill the application, change the condition to 0 to enable this branch.
    Fl::run();
#endif
    return quit_pushed;
}
void My_window::cb_add(Address, Address pw)
{
    reference_to<My_window>(pw).addi();
}

void My_window::addi()
{
    int x = next_x.get_int();
    int y = next_y.get_int();
    Point p(x, y + 40);
    area.add(p);
    circles.add(p);
    circles.set_color(Graph_lib::Color::red);
    attach(circles);
    redraw();
}

void My_window::cb_next(Address, Address pw)
{
    reference_to<My_window>(pw).next();
}

void My_window::next()
{
    if(next_pushed)
        return;

    if(!area.check())
    {
        s_pixel.put("Error Polygon");
        s_true.put("Error Polygon");
        return;
    }
    area.calculate();
    next_pushed = true;
    area.set_color(Graph_lib::Color::black);
    area.set_fill_color(Graph_lib::Color::white);
    attach(area);
//    s_pixel.put(format_scientific(area.area, 5));
    // s_true.put(format_scientific(area.area * rr * rr, 5));

    s_pixel.put(pointToString(area.ans));
    s_true.put(format_scientific(area.ans_area, 5));
    c.push_back(new Circle(area.ans,2));
    c[0].set_color(Color::red);
    attach(c[0]);

    for(int _y = 40; _y <= y_max(); _y += 3)
    {
        vector<pair<int, int> > line_p;
        Point p0(100, 100);
        for(int i = 0; i < area.ans_list.size() - 1; i++)
        {
            if(line_segment_intersect_w(Point(0, _y), Point(x_max(), _y),area.ans_list[i], area.ans_list[i + 1], p0))
                line_p.push_back(make_pair(p0.x, p0.y));
        }
        if(line_segment_intersect_w(Point(0, _y), Point(x_max(), _y),area.ans_list[area.ans_list.size()-1], area.ans_list[0], p0))
            line_p.push_back(make_pair(p0.x, p0.y));

        sort(line_p.begin(), line_p.end(), cmp);
        int num = line_p.size();
        for(int i = 0; i < num - 1; i += 2)
            lines.add(Point(line_p[i].first + 3, line_p[i].second),Point(line_p[i + 1].first - 3, line_p[i + 1].second));
    }
    lines.set_color(Color::yellow);
    attach(lines);


    detach(add_button);
    detach(next_button);
    detach(next_x);
    detach(next_y);
    detach(circles);
    redraw();
}
void My_window::cb_quit(Address, Address pw)
{
    reference_to<My_window>(pw).quit();
}

void My_window::quit()
{
    quit_pushed = true;
    hide();
}


void My_window::cb_nxt(Address, Address pw)
{
    reference_to<My_window>(pw).nxt();
}

void My_window::nxt()
{
    detach(nxt_button);
    attach(csc);
    attach(add_button);
    attach(next_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(s_pixel);
    attach(s_true);
    redraw();
}
void My_window::cb_csc(Address, Address pw)
{
    reference_to<My_window>(pw).cscf();
}
void My_window::cscf()
{
    redraw();
}