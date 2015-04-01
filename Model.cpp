#include "Model.h"
#include "Ship.h"
#include "Island.h"
#include "View.h"
#include "Ship_factory.h"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace placeholders;

const char* const ISLAND_NOT_FOUND_MSG = "Island not found!";
const char* const SHIP_NOT_FOUND_MSG = "Ship not found!";

Model *Model::model = 0;

Model* Model::get_Instance() {
    if (!model) model = new Model;
    return model;
}

// create the initial objects, output constructor message
Model::Model() : time(0)
{
    Model::Island_ptr exxon = make_shared<Island>("Exxon", Point(10, 10), 1000, 200);
    Model::Island_ptr shell = make_shared<Island>("Shell", Point(0, 30), 1000, 200);
    Model::Island_ptr bermuda = make_shared<Island>("Bermuda", Point(20, 20));
    Model::Island_ptr treasure_island = make_shared<Island>("Treasure_Island", Point(50, 5), 100, 5);

    string short_exxon = exxon->get_name().substr(0, SHORTEN_NAME_LENGTH);
    islands[short_exxon] = exxon;
    objects[short_exxon] = exxon;
    string short_shell = shell->get_name().substr(0, SHORTEN_NAME_LENGTH);
    islands[short_shell] = shell;
    objects[short_shell] = shell;
    string short_bermuda = bermuda->get_name().substr(0, SHORTEN_NAME_LENGTH);
    islands[short_bermuda] = bermuda;
    objects[short_bermuda] = bermuda;
    string short_treasure_island = treasure_island->get_name().substr(0, SHORTEN_NAME_LENGTH);
    islands[short_treasure_island] = treasure_island;
    objects[short_treasure_island] = treasure_island;

    add_ship(create_ship("Ajax", "Cruiser", Point (15, 15)));
    add_ship(create_ship("Xerxes", "Cruiser", Point (25, 25)));
    add_ship(create_ship("Valdez", "Tanker", Point (30, 30)));

    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Model constructed" << endl;
}

// destroy all objects, output destructor message
Model::~Model()
{
    objects.clear();
    ships.clear();
    islands.clear();
    views.clear();
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Model destructed" << endl;
}

// will throw Error("Island not found!") if no island of that name
Model::Island_ptr Model::get_island_ptr(const std::string& name) const
{
    string shortened_name = name.substr(0, SHORTEN_NAME_LENGTH);
    auto island_it = islands.find(shortened_name);
    if (island_it == islands.end()) throw Error(ISLAND_NOT_FOUND_MSG);
    Model::Island_ptr island = (*island_it).second;
    if (island->get_name() != name) throw Error(ISLAND_NOT_FOUND_MSG);
    return island;
}

// add a new ship to the list, and update the view
void Model::add_ship(Model::Ship_ptr ship)
{
    string shortened_name = ship->get_name().substr(0, SHORTEN_NAME_LENGTH);
    ships[shortened_name] = ship;
    objects[shortened_name] = ship;
    notify_location_ship(ship->get_name(), ship->get_location());
}
// will throw Error("Ship not found!") if no ship of that name
Model::Ship_ptr Model::get_ship_ptr(const std::string& name) const
{
    string shortened_name = name.substr(0, SHORTEN_NAME_LENGTH);
    auto ship_it = ships.find(shortened_name);
    if (ship_it == ships.end()) throw Error(SHIP_NOT_FOUND_MSG);
    Ship_ptr ship = (*ship_it).second;
    if (ship->get_name() != name) throw Error(SHIP_NOT_FOUND_MSG);
    return ship;
}
void Model::remove_ship(shared_ptr<Ship> ship)
{
    string shortened_name = ship->get_name().substr(0, SHORTEN_NAME_LENGTH);
    auto ship_it = ships.find(shortened_name);
    if (ship_it == ships.end()) throw Error(SHIP_NOT_FOUND_MSG);
    ships.erase(ship_it);
    objects.erase(shortened_name);
}

// tell all objects to describe themselves
void Model::describe() const
{
    for_each(objects.begin(), objects.end(), [](pair<string, Sim_object_ptr> pair){pair.second->describe();});
}
// increment the time, and tell all objects to update themselves
void Model::update()
{
    ++time;
    for_each(objects.begin(), objects.end(), [](pair<string, Sim_object_ptr> pair){pair.second->update();});
}

/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects' locations (or other state information).
void Model::attach(shared_ptr<View> view)
{
    views.push_back(view);
    for_each(objects.begin(), objects.end(), [view](pair<string, Sim_object_ptr> pair){pair.second->broadcast_current_state();});
}
// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(shared_ptr<View> view)
{
    views.erase(find(views.begin(), views.end(), view));
}

// notify the views about a ship's location
void Model::notify_location_ship(const std::string &name, Point location)
{
    for_each(views.begin(), views.end(), bind(&View::update_location_ship, _1, name, location));
}
// notify the views about an island's location
void Model::notify_location_island(const std::string &name, Point location)
{
    for_each(views.begin(), views.end(), bind(&View::update_location_island, _1, name, location));
}
// notify the views that an object is now gone
void Model::notify_gone(const std::string& name)
{
    for_each(views.begin(), views.end(), bind(&View::update_remove_ship, _1, name));
}
// notify the views that a ship has changed fuel
void Model::notify_fuel(const std::string& name, double fuel)
{
    for_each(views.begin(), views.end(), bind(&View::update_fuel, _1, name, fuel));
}
// notify the views that a ship has changed course and speed
void Model::notify_course_speed(const std::string& name, double course, double speed)
{
    for_each(views.begin(), views.end(), bind(&View::update_course_and_speed, _1, name, course, speed));
}