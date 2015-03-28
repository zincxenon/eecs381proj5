#include "View_sail.h"
#include <iostream>

using namespace std;

View_sail::View_sail()
{
    cout << "View_sail constructed" << endl;
}
View_sail::~View_sail()
{
    cout << "View_sail destructed" << endl;
}

// Save the supplied name and information for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void View_sail::update_location_ship(const std::string& name, Point location)
{
    auto ship_it = ship_map.find(name);
    if (ship_it == ship_map.end())
    {
        Ship_data data;
        data.location = location;
        ship_map[name] = data;
    }
    else
    {
        (*ship_it).second.location = location;
    }
}
void View_sail::update_course_and_speed(const std::string& name, double course, double speed)
{
    auto ship_it = ship_map.find(name);
    if (ship_it == ship_map.end())
    {
        Ship_data data;
        data.course = course;
        data.speed = speed;
        ship_map[name] = data;
    }
    else
    {
        (*ship_it).second.course = course;
        (*ship_it).second.speed = speed;
    }
}
void View_sail::update_fuel(const std::string& name, double fuel)
{
    auto ship_it = ship_map.find(name);
    if (ship_it == ship_map.end())
    {
        Ship_data data;
        data.fuel = fuel;
        ship_map[name] = data;
    }
    else
    {
        (*ship_it).second.fuel = fuel;
    }
}

// Remove the name and its location; no error if the name is not present.
void View_sail::update_remove_ship(const std::string& name)
{
    auto ship_it = ship_map.find(name);
    if (ship_it != ship_map.end()) ship_map.erase(ship_it);
}

// prints out the current map
void View_sail::draw()
{

}

// Discard the saved information - drawing will show only a empty pattern
void View_sail::clear()
{

}
