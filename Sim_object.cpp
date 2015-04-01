#include "Sim_object.h"
#include "Utility.h"
#include <iostream>

using namespace std;

Sim_object::Sim_object(const std::string& name_) : name(name_)
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Sim_object " << get_name() << " constructed" << endl;
}

Sim_object::~Sim_object()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Sim_object " << get_name() << " destructed" << endl;
}