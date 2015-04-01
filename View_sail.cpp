#include "View_sail.h"
#include <iostream>
#include <iomanip>

using namespace std;

const int VIEW_SAIL_FIELD_SIZE = 10;

View_sail::View_sail() : View()
{
    cout << "View_sail constructed" << endl;
}
View_sail::~View_sail()
{
    cout << "View_sail destructed" << endl;
}

void View_sail::update_course_and_speed(const std::string& name, double course, double speed)
{
    auto ship_it = ship_map.find(name);
    if (ship_it == ship_map.end())
    {
        Ship_data data;
        data.set_course_speed(course, speed);
        ship_map[name] = data;
    }
    else
    {
        (*ship_it).second.set_course_speed(course, speed);
    }
}
void View_sail::update_fuel(const std::string& name, double fuel)
{
    auto ship_it = ship_map.find(name);
    if (ship_it == ship_map.end())
    {
        Ship_data data;
        data.set_fuel(fuel);
        ship_map[name] = data;
    }
    else
    {
        (*ship_it).second.set_fuel(fuel);
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
    cout << "----- Sailing Data -----" << endl;
    auto old_width = cout.width();
    cout.width(VIEW_SAIL_FIELD_SIZE);
    cout << "Ship" << "Fuel" << "Course" << "Speed" << endl;
    for (auto&& ship : ship_map)
    {
        cout << ship.first << ship.second.get_fuel() << ship.second.get_course() << ship.second.get_speed() << endl;
    }
    cout.width(old_width);
}

// Discard the saved information - drawing will show only a empty pattern
void View_sail::clear()
{
    ship_map.clear();
}
