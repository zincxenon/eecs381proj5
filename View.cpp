#include "View.h"
#include "Utility.h"
#include <iostream>

using namespace std;

View::View()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View constructed" << endl;
}

View::~View()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View destructed" << endl;
}