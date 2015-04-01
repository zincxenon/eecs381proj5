#include "View_locations.h"
#include <iostream>

using namespace std;

View_locations::View_locations() : View()
{
    cout << "View_locations constructed" << endl;
}
View_locations::~View_locations()
{
    cout << "View_locations destructed" << endl;
}

// Save the supplied name and information for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void View_locations::update_location_ship(const std::string& name, Point location)
{
    object_data[name] = location;
}
void View_locations::update_location_island(const std::string& name, Point location)
{
    object_data[name] = location;
}

// Remove the name and its location; no error if the name is not present.
void View_locations::update_remove_ship(const std::string& name)
{
    auto object_it = object_data.find(name);
    if (object_it != object_data.end()) object_data.erase(object_it);
}