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

bool cmp(pair<int, int> p1, pair<int, int> p2)
{
    return p1.first < p2.first;
}

My_window::My_window(Point xy, int w, int h, const string& title) :
        Window(xy,w,h,title),
        address(Point(75, 0), x_max() / 2 - 200, 20, " Address :  "),
        ratio(Point(x_max() / 2 + 50, 0), x_max() / 2 - 200, 20, "Ratio(pixel:m): "),
        nxt_button(Point(x_max() - 100, 0), 100, 20, "load: ",cb_nxt),
        add_button(Point(x_max()-300, 20),100, 20, "Add", cb_add),
        next_button(Point(x_max()-200,20), 100, 20, "Draw", cb_next),
        next_pushed(false),
        quit_button(Point(x_max()-100,20), 100, 20, "Quit", cb_quit),
        quit_pushed(false),
        s_pixel(Point(75, 20), 150, 20, "像素面积："),
        s_true(Point(325, 20), 150, 20, "实际面积："),
        next_x(Point(575, 20), 100, 20, "输入 x："),
        next_y(Point(775, 20), 100, 20, "输入 y：")
{
    attach(address);
    attach(ratio);
    attach(nxt_button);
    attach(add_button);
    attach(next_button);
    attach(quit_button);
    attach(next_x);
    attach(next_y);
    attach(s_pixel);
    attach(s_true);
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
    next_pushed = true;
    area.set_color(Graph_lib::Color::red);
    attach(area);
//    s_pixel.put(to_string(area.area));
//    s_true.put(to_string(area.area * rr * rr));
    s_pixel.put(format_scientific(area.area, 5));
    s_true.put(format_scientific(area.area * rr * rr, 5));
    for(int _y = 40; _y <= y_max(); _y += 3)
    {
        vector<pair<int, int> > line_p;
        Point p0(100, 100);
        for(int i = 0; i < area.number_of_points() - 1; i++)
        {
            if(line_segment_intersect_w(Point(0, _y), Point(x_max(), _y),area.point(i), area.point(i + 1), p0))
                line_p.push_back(make_pair(p0.x, p0.y));
        }
        if(line_segment_intersect_w(Point(0, _y), Point(x_max(), _y),area.point(area.number_of_points() - 1), area.point(0), p0))
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
    string addr = address.get_string();
    string rat = ratio.get_string();
    //ii.push_back(new Image(Point(0, 40), addr));
    ii.push_back(new Image(Point(0, 40),"/Users/miraclecsc/CLionProjects/FLTK/Project1/src/3.jpg"));
    attach(ii[0]);
    int p = 0, q = 0;
    bool flag = true;
    for(auto c : rat)
    {
        if(c == ':')
        {
            flag = false;
            continue;
        }
        if(flag)
            p = p * 10 + c - '0';
        else
            q = q * 10 + c - '0';
    }
    rr = double(q) / double(p);
    //s_true.put(to_string(rr));
    detach(nxt_button);
    redraw();
}