#include "Controller.h"

#include "Model.h"
#include "View.h"
#include "View_map.h"
#include "View_sail.h"
#include "View_bridge.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>
#include <limits>

using namespace std;
using namespace placeholders;

const char* const UNRECOGNIZED_ERROR_MSG = "Unrecognized command!";
const int MAX_COURSE_DEGREES = 360;

// output constructor message
Controller::Controller()
{
    cout << "Controller constructed" << endl;
}
// output destructor message
Controller::~Controller()
{
    cout << "Controller destructed" << endl;
}

// create View object, run the program by accepting user commands, then destroy View object
void Controller::run()
{
    while (true)
    {
        try
        {
            cout << "\nTime " << Model::get_Instance()->get_time() << ": Enter command: ";
            string command;
            cin >> command;
            // first, check the command func map if this a command word
            if (command_func_map.find(command) != command_func_map.end())
            {
                // if so, run function and return if it returns true
                if ((this->*command_func_map[command])()) return;
            }
            else
            {
                // if not, find the ship with this name
                shared_ptr<Ship> ship;
                try { ship = Model::get_Instance()->get_ship_ptr(command); }
                catch (Error& e) { throw Error(UNRECOGNIZED_ERROR_MSG); }
                // if there is no ship with that name, an unrecognized command error will be thrown
                // read in the ship command
                string ship_command;
                cin >> ship_command;
                // check the ship func map if this is a command word
                if (ship_func_map.find(ship_command) != ship_func_map.end())
                {
                    // if so, run the function
                    (this->*ship_func_map[ship_command])(ship);
                }
                else
                {
                    // if not, throw an unrecognized command error
                    throw Error(UNRECOGNIZED_ERROR_MSG);
                }
            }
        }
        catch (Error& e)
        {
            cout << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (...)
        {
            cout << "An unknown error occurred!" << endl;
            return;
        }
    }
}

// helper functions
int Controller::read_int()
{
    int new_int;
    if (!(cin >> new_int)) throw Error("Expected an integer!");
    return new_int;
}
double Controller::read_double()
{
    double new_double;
    if (!(cin >> new_double)) throw Error("Expected a double!");
    return new_double;
}
double Controller::read_speed()
{
    double speed = read_double();
    if (speed < 0) throw Error("Negative speed entered!");
    return speed;
}
shared_ptr<Ship> Controller::read_ship()
{
    string name;
    cin >> name;
    return Model::get_Instance()->get_ship_ptr(name);
}
shared_ptr<Island> Controller::read_island()
{
    string name;
    cin >> name;
    return Model::get_Instance()->get_island_ptr(name);
}

// command functions
bool Controller::quit()
{
    Model *model = Model::get_Instance();
    for_each(views.begin(), views.end(), [model](weak_ptr<View> view){model->detach(view.lock());});
    cout << "Done" << endl;
    return true;
}

// view functions
bool Controller::view_show()
{
    // first, remove all expired views from the list of views
    views.erase(remove_if(views.begin(), views.end(), mem_fn(&weak_ptr<View>::expired)));
    // now, draw every view
    for_each(views.begin(), views.end(), [](weak_ptr<View> view){view.lock()->draw();});
    return false;
}
bool Controller::view_map_open()
{
    if (view_map) throw Error("Map view is already open!");
    view_map = make_shared<View_map>();
    shared_ptr<View> view_ptr = dynamic_pointer_cast<View, View_map>(view_map);
    Model::get_Instance()->attach(view_ptr);
    views.push_back(weak_ptr<View>(view_ptr));
    return false;
}
bool Controller::view_map_close()
{
    if (!view_map) throw Error("Map view is not open!");
    Model::get_Instance()->detach(dynamic_pointer_cast<View, View_map>(view_map));
    view_map.reset();
    return false;
}
bool Controller::view_sail_open()
{
    if (view_sail) throw Error("Sailing data view is already open!");
    view_sail = make_shared<View_sail>();
    shared_ptr<View> view_ptr = dynamic_pointer_cast<View, View_sail>(view_sail);
    Model::get_Instance()->attach(view_ptr);
    views.push_back(weak_ptr<View>(view_ptr));
    return false;
}
bool Controller::view_sail_close()
{
    if (!view_sail) throw Error("Sailing data view is not open!");
    Model::get_Instance()->detach(dynamic_pointer_cast<View, View_sail>(view_sail));
    view_sail.reset();
    return false;
}
bool Controller::view_bridge_open()
{
    string name;
    cin >> name;
    if (bridge_views.find(name) != bridge_views.end()) throw Error("Bridge view is already open for that ship!");
    shared_ptr<View_bridge> bridge_view = make_shared<View_bridge>(name);
    bridge_views[name] = bridge_view;
    shared_ptr<View> view_ptr = dynamic_pointer_cast<View, View_bridge>(bridge_view);
    Model::get_Instance()->attach(view_ptr);
    views.push_back(view_ptr);
    return false;
}
bool Controller::view_bridge_close()
{
    string name;
    cin >> name;
    auto view_it = bridge_views.find(name);
    if (view_it == bridge_views.end()) throw Error("Bridge view for that ship is not open!");
    Model::get_Instance()->detach(dynamic_pointer_cast<View, View_bridge>((*view_it).second));
    bridge_views.erase(view_it);
    return false;
}

// view_map functions
bool Controller::view_map_default()
{
    if (!view_map) throw Error("Map view is not open!");
    view_map->set_defaults();
    return false;
}
bool Controller::view_map_size()
{
    int new_size = read_int();
    if (!view_map) throw Error("Map view is not open!");
    view_map->set_size(new_size);
    return false;
}
bool Controller::view_map_zoom()
{
    double new_scale = read_double();
    if (!view_map) throw Error("Map view is not open!");
    view_map->set_scale(new_scale);
    return false;
}
bool Controller::view_map_pan()
{
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    if (!view_map) throw Error("Map view is not open!");
    view_map->set_origin(Point(point_x, point_y));
    return false;
}

// model functions
bool Controller::model_status()
{
    Model::get_Instance()->describe();
    return false;
}
bool Controller::model_go()
{
    Model::get_Instance()->update();
    return false;
}
bool Controller::model_create()
{
    string new_name;
    cin >> new_name;
    if (new_name.size() < SHORTEN_NAME_LENGTH) throw Error("Name is too short!");
    if (Model::get_Instance()->is_name_in_use(new_name)) throw Error("Name is already in use!");
    string new_type;
    cin >> new_type;
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    Model::get_Instance()->add_ship(create_ship(new_name, new_type, Point(point_x, point_y)));
    return false;
}

// ship functions
void Controller::ship_course(shared_ptr<Ship> ship)
{
    assert(ship);
    double course = read_double();
    if (course < 0 || course >= MAX_COURSE_DEGREES) throw Error("Invalid heading entered!");
    ship->set_course_and_speed(course, read_speed());
}
void Controller::ship_position(shared_ptr<Ship> ship)
{
    assert(ship);
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    ship->set_destination_position_and_speed(Point(point_x, point_y), read_speed());
}
void Controller::ship_destination(shared_ptr<Ship> ship)
{
    assert(ship);
    shared_ptr<Island> destination = read_island();
    ship->set_destination_position_and_speed(destination->get_location(), read_speed());
}
void Controller::ship_load_at(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->set_load_destination(read_island());
}
void Controller::ship_unload_at(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->set_unload_destination(read_island());
}
void Controller::ship_dock_at(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->dock(read_island());
}
void Controller::ship_attack(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->attack(read_ship());
}
void Controller::ship_refuel(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->refuel();
}
void Controller::ship_stop(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->stop();
}
void Controller::ship_stop_attack(shared_ptr<Ship> ship)
{
    assert(ship);
    ship->stop_attack();
}