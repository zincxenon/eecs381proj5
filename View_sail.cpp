#include "View_sail.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>

using namespace std;

const int VIEW_SAIL_FIELD_SIZE = 10;

View_sail::View_sail() : View()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_sail constructed" << endl;
}
View_sail::~View_sail()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_sail destructed" << endl;
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
    cout.width(VIEW_SAIL_FIELD_SIZE);
    cout << setw(VIEW_SAIL_FIELD_SIZE) << "Ship" << setw(VIEW_SAIL_FIELD_SIZE) << "Fuel" <<
            setw(VIEW_SAIL_FIELD_SIZE) << "Course" << setw(VIEW_SAIL_FIELD_SIZE) << "Speed" << endl;
    for (auto&& ship : ship_map)
    {
        cout << setw(VIEW_SAIL_FIELD_SIZE) << ship.first << setw(VIEW_SAIL_FIELD_SIZE) << ship.second.get_fuel() <<
                setw(VIEW_SAIL_FIELD_SIZE) << ship.second.get_course() <<
                setw(VIEW_SAIL_FIELD_SIZE) << ship.second.get_speed() << endl;
    }
}

// Discard the saved information - drawing will show only a empty pattern
void View_sail::clear()
{
    ship_map.clear();
}
