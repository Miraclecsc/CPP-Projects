#include "My_window.h"
#include "Simple_window.h"
#include "Graph.h"
using namespace std;
using namespace Graph_lib;
int main()
{
    My_window win(Point(100,100),1200,700,"Canvas");
    win.wait_for_button();
}

