#include "Controller.h"

#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"
#include "Utility.h"
#include <string>
#include <iostream>
#include <map>
#include <limits>
#include <memory>

using namespace std;

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
    shared_ptr<View> view(make_shared<View>()); // make map view here
    g_Model_ptr->attach(view);
    while (true)
    {
        try
        {
            cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
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
                try { ship = g_Model_ptr->get_ship_ptr(command); }
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
    return g_Model_ptr->get_ship_ptr(name);
}
shared_ptr<Island> Controller::read_island()
{
    string name;
    cin >> name;
    return g_Model_ptr->get_island_ptr(name);
}

// command functions
bool Controller::quit()
{
    g_Model_ptr->detach(view);
    delete view;
    cout << "Done" << endl;
    return true;
}

// view functions
bool Controller::view_default()
{
    view->set_defaults();
    return false;
}
bool Controller::view_size()
{
    view->set_size(read_int());
    return false;
}
bool Controller::view_zoom()
{
    view->set_scale(read_double());
    return false;
}
bool Controller::view_pan()
{
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    view->set_origin(Point(point_x, point_y));
    return false;
}
bool Controller::view_show()
{
    view->draw();
    return false;
}

// model functions
bool Controller::model_status()
{
    g_Model_ptr->describe();
    return false;
}
bool Controller::model_go()
{
    g_Model_ptr->update();
    return false;
}
bool Controller::model_create()
{
    string new_name;
    cin >> new_name;
    if (new_name.size() < SHORTEN_NAME_LENGTH) throw Error("Name is too short!");
    if (g_Model_ptr->is_name_in_use(new_name)) throw Error("Name is already in use!");
    string new_type;
    cin >> new_type;
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    g_Model_ptr->add_ship(create_ship(new_name, new_type, Point(point_x, point_y)));
    return false;
}

// ship functions
void Controller::ship_course(shared_ptr<Ship> ship)
{
    double course = read_double();
    if (course < 0 || course >= MAX_COURSE_DEGREES) throw Error("Invalid heading entered!");
    ship->set_course_and_speed(course, read_speed());
}
void Controller::ship_position(shared_ptr<Ship> ship)
{
    double point_x, point_y;
    point_x = read_double();
    point_y = read_double();
    ship->set_destination_position_and_speed(Point(point_x, point_y), read_speed());
}
void Controller::ship_destination(shared_ptr<Ship> ship)
{
    Island *destination = read_island();
    ship->set_destination_position_and_speed(destination->get_location(), read_speed());
}
void Controller::ship_load_at(shared_ptr<Ship> ship)
{
    ship->set_load_destination(read_island());
}
void Controller::ship_unload_at(shared_ptr<Ship> ship)
{
    ship->set_unload_destination(read_island());
}
void Controller::ship_dock_at(shared_ptr<Ship> ship)
{
    ship->dock(read_island());
}
void Controller::ship_attack(shared_ptr<Ship> ship)
{
    ship->attack(read_ship());
}
void Controller::ship_refuel(shared_ptr<Ship> ship)
{
    ship->refuel();
}
void Controller::ship_stop(shared_ptr<Ship> ship)
{
    ship->stop();
}
void Controller::ship_stop_attack(shared_ptr<Ship> ship)
{
    ship->stop_attack();
}