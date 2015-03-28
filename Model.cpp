#include "Model.h"
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include "Ship.h"
#include "Island.h"
#include "View.h"
#include "Geometry.h"
#include "Ship_factory.h"

using namespace std;
using namespace placeholders;

const char* const ISLAND_NOT_FOUND_MSG = "Island not found!";
const char* const SHIP_NOT_FOUND_MSG = "Ship not found!";

Model* Model::get_Instance() {
    if (!model) model = new Model;
    return model;
}

// create the initial objects, output constructor message
Model::Model() : time(0)
{
    Island_ptr exxon = new Island("Exxon", Point(10, 10), 1000, 200);
    Island_ptr shell = new Island("Shell", Point(0, 30), 1000, 200);
    Island_ptr bermuda = new Island("Bermuda", Point(20, 20));
    Island_ptr treasure_island = new Island("Treasure Island", Point(50, 5), 100, 5);

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

    cout << "Model constructed" << endl;
}

// destroy all objects, output destructor message
Model::~Model()
{
    for_each(objects.begin(), objects.end(), [](pair<string, Sim_object_ptr> pair){delete pair.second;});
    cout << "Model destructed" << endl;
}

// will throw Error("Island not found!") if no island of that name
Island_ptr Model::get_island_ptr(const std::string& name) const
{
    string shortened_name = name.substr(0, SHORTEN_NAME_LENGTH);
    auto island_it = islands.find(shortened_name);
    if (island_it == islands.end()) throw Error(ISLAND_NOT_FOUND_MSG);
    Island_ptr island = (*island_it).second;
    if (island->get_name() != name) throw Error(ISLAND_NOT_FOUND_MSG);
    return island;
}

// add a new ship to the list, and update the view
void Model::add_ship(Ship_ptr ship)
{
    string shortened_name = ship->get_name().substr(0, SHORTEN_NAME_LENGTH);
    ships[shortened_name] = ship;
    objects[shortened_name] = ship;
    notify_location(ship->get_name(), ship->get_location());
}
// will throw Error("Ship not found!") if no ship of that name
Ship_ptr Model::get_ship_ptr(const std::string& name) const
{
    string shortened_name = name.substr(0, SHORTEN_NAME_LENGTH);
    auto ship_it = ships.find(shortened_name);
    if (ship_it == ships.end()) throw Error(SHIP_NOT_FOUND_MSG);
    Ship *ship = (*ship_it).second;
    if (ship->get_name() != name) throw Error(SHIP_NOT_FOUND_MSG);
    return ship;
}
void Model::remove_ship(shared_ptr<Ship> ship)
{
    auto ship_it = get_ship_ptr(ship->get_name());
    // calling get_ship_ptr performs a check on the validity of the ptr
    ships.erase(ship_it);
    objects.erase(ship->get_name().substr(0, SHORTEN_NAME_LENGTH));
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
    for_each(objects.begin(), objects.end(), [view](pair<string, Sim_object_ptr> pair){view->update_location(pair.second->get_name(), pair.second->get_location());});
}
// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(shared_ptr<View> view)
{
    views.erase(find(views.begin(), views.end(), view));
}

// notify the views about an object's location
void Model::notify_location(const std::string& name, Point location)
{
    for_each(views.begin(), views.end(), bind(&View::update_location, _1, name, location));
}
// notify the views that an object is now gone
void Model::notify_gone(const std::string& name)
{
    for_each(views.begin(), views.end(), bind(&View::update_remove, _1, name));
}
