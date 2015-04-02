#include "Views.h"
#include "Navigation.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

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

View_locations::View_locations() : View()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_locations constructed" << endl;
}
View_locations::~View_locations()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_locations destructed" << endl;
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

const int VIEW_BRIDGE_MAP_HEIGHT = 3;
const int VIEW_BRIDGE_MAP_WIDTH = 19;
const string VIEW_BRIDGE_MULTIPLE_OBJECT = "**";
const string VIEW_BRIDGE_NO_OBJECT = ". ";
const string VIEW_BRIDGE_UNDERWATER = "w-";
const int VIEW_BRIDGE_AXIS_LABEL_MAX = 4;
const int VIEW_BRIDGE_LINES_PER_AXIS_LABEL = 3;

const double VIEW_BRIDGE_MIN_SHOW = -90;
const double VIEW_BRIDGE_SCALE = 10;
const double VIEW_BRIDGE_MAX_DIST = 20;
const double VIEW_BRIDGE_MIN_DIST = .005;

const double VIEW_BRIDGE_FULL = 360;
const double VIEW_BRIDGE_HALF = 180;

View_bridge::View_bridge(const std::string& name) : View_locations(), target(name), target_sunk(false)
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_bridge constructed" << endl;
}
View_bridge::~View_bridge()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_bridge destructed" << endl;
}

void View_bridge::update_course_and_speed(const std::string& name, double course, double speed)
{
    if (name == target)
    {
        target_course = course;
    }
}

void View_bridge::update_remove_ship(const std::string& name)
{
    if (name == target)
    {
        assert(object_data.find(target) != object_data.end());
        target_sunk = true;
        target_location = object_data[name];
    }
    View_locations::update_remove_ship(name);
}

// prints out the view
void View_bridge::draw()
{
    // initialize the bridge map
    vector<vector<string>> bridge_map;
    // a row in the map must start out filled only with VIEW_BRIDGE_NO_OBJECT or VIEW_BRIDGE_UNDERWATER
    vector<string> initial_row;
    string default_string = target_sunk ? VIEW_BRIDGE_UNDERWATER : VIEW_BRIDGE_NO_OBJECT;
    for (int i = 0; i < VIEW_BRIDGE_MAP_WIDTH; i++) initial_row.push_back(default_string);
    // fill the map with copies of the initial row
    for (int i = 0; i < VIEW_BRIDGE_MAP_HEIGHT; i++) bridge_map.push_back(vector<string>(initial_row));

    // if sunk, display the watery pattern, otherwise build the map
    if (target_sunk)
    {
        cout << "Bridge view from " << target << " sunk at " << target_location << endl;
    }
    else
    {
        // if not sunk then the object must appear in the object data map
        assert(object_data.find(target) != object_data.end());
        cout << "Bridge view from " << target << " position " << object_data[target] << " heading " << target_course << endl;

        // build the bridge map
        for (auto&& object : object_data)
        {
            int x;
            if (get_heading(x, object.second))
            {
                if (bridge_map[0][x] == VIEW_BRIDGE_NO_OBJECT) bridge_map[0][x] = object.first.substr(0, SHORTEN_NAME_LENGTH);
                else bridge_map[0][x] = VIEW_BRIDGE_MULTIPLE_OBJECT;
            }
        }
    }
    // display the map
    for (int y = VIEW_BRIDGE_MAP_HEIGHT - 1; y >= 0; y--)
    {
        cout << setw(VIEW_BRIDGE_AXIS_LABEL_MAX + 1) << " ";
        // iterate through the map of this row
        for (int x = 0; x < VIEW_BRIDGE_MAP_WIDTH; x++)
        {
            cout << bridge_map[y][x];
        }
        cout << endl;
    }
    // save precision
    int old_precision = cout.precision();
    cout.precision(0);
    // display the axis labels
    for (int x = 0; x < VIEW_BRIDGE_MAP_WIDTH; x += VIEW_BRIDGE_LINES_PER_AXIS_LABEL)
    {
        cout << setw(SHORTEN_NAME_LENGTH * VIEW_BRIDGE_LINES_PER_AXIS_LABEL) << (VIEW_BRIDGE_MIN_SHOW + x * VIEW_BRIDGE_SCALE);
    }
    cout << endl;
    // restore precision
    cout.precision(old_precision);
}

bool View_bridge::get_heading(int& x, Point location)
{
    Compass_position compass(object_data[target], location);
    if (compass.range < VIEW_BRIDGE_MIN_DIST || compass.range > VIEW_BRIDGE_MAX_DIST) return false;
    double bearing = compass.bearing;
    cout << "original bearing to position " << location << " is " << bearing << endl;
    if (bearing < -1 * VIEW_BRIDGE_HALF)
    {
        bearing += VIEW_BRIDGE_FULL;
    }
    if (bearing > VIEW_BRIDGE_HALF)
    {
        bearing -= VIEW_BRIDGE_HALF;
    }
    cout << "bearing to position " << location << " is " << bearing << endl;
    x = int(floor((bearing - VIEW_BRIDGE_MIN_SHOW) / VIEW_BRIDGE_SCALE));
    if (x < 0 || x >= VIEW_BRIDGE_MAP_WIDTH) return false;
    return true;
}

const int VIEW_MAP_DEFAULT_SIZE = 25;
const double VIEW_MAP_DEFAULT_SCALE = 2;
const Point VIEW_MAP_DEFAULT_ORIGIN(-10, -10);
const string VIEW_MAP_MULTIPLE_OBJECT = "* ";
const string VIEW_MAP_NO_OBJECT = ". ";
const int VIEW_MAP_LINES_PER_AXIS_LABEL = 3;
const int VIEW_MAP_AXIS_LABEL_MAX = 4;

// default constructor sets the default size, scale, and origin, outputs constructor message
View_map::View_map() : View_locations()
{
    set_defaults();
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_map constructed" << endl;
}
// outputs destructor message
View_map::~View_map()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "View_map destructed" << endl;
}

// prints out the current map
void View_map::draw()
{
    // save precision
    int old_precision = cout.precision();

    vector<vector<string>> map_view;
    vector<string> objects_out_of_map;
    // a row in the map must start out filled only with VIEW_MAP_NO_OBJECTs
    vector<string> initial_row;
    for (int i = 0; i < size; i++) initial_row.push_back(VIEW_MAP_NO_OBJECT);
    // fill the map with copies of the initial row
    for (int i = 0; i < size; i++) map_view.push_back(vector<string>(initial_row));

    // generate the map
    // each object in object_data is a pair where first is the name and second is the Point
    for (auto&& object : object_data)
    {
        int x, y;
        if (get_subscripts(x, y, object.second))
        {
            if (map_view[y][x] == VIEW_MAP_NO_OBJECT) map_view[y][x] = object.first.substr(0, SHORTEN_NAME_LENGTH);
            else map_view[y][x] = VIEW_MAP_MULTIPLE_OBJECT;
        } else objects_out_of_map.push_back(object.first);
    }

    cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
    cout.precision(0);
    if (objects_out_of_map.size() > 0)
    {
        for (size_t i = 0; i < objects_out_of_map.size() - 1; i++)
        {
            cout << objects_out_of_map[i] << ", ";
        }
        cout << objects_out_of_map[objects_out_of_map.size() - 1] << " outside the map" << endl;
    }
    // start from max y and iterate through map
    for (int y = size - 1; y >= 0; y--)
    {
        if (y % VIEW_MAP_LINES_PER_AXIS_LABEL == 0)
        {
            cout << setw(VIEW_MAP_AXIS_LABEL_MAX) << (origin.y + scale * y) << " ";
        } else cout << setw(VIEW_MAP_AXIS_LABEL_MAX + 1) << " ";
        // iterate through the map of this row
        for (int x = 0; x < size; x++)
        {
            cout << map_view[y][x];
        }
        cout << endl;
    }
    for (int x = 0; x < size; x += VIEW_MAP_LINES_PER_AXIS_LABEL)
    {
        cout << setw(SHORTEN_NAME_LENGTH * VIEW_MAP_LINES_PER_AXIS_LABEL) << (origin.x + scale * x);
    }
    cout << endl;

    // restore precision
    cout.precision(old_precision);
}

// modify the display parameters
// if the size is out of bounds will throw Error("New map size is too big!")
// or Error("New map size is too small!")
void View_map::set_size(int size_)
{
    if (size_ <= 6) throw Error("New map size is too small!");
    if (size_ > 30) throw Error("New map size is too big!");
    size = size_;
}

// If scale is not positive, will throw Error("New map scale must be positive!");
void View_map::set_scale(double scale_)
{
    if (scale_ <= 0) throw Error("New map scale must be positive!");
    scale = scale_;
}

// set the parameters to the default values
void View_map::set_defaults()
{
    size = VIEW_MAP_DEFAULT_SIZE;
    scale = VIEW_MAP_DEFAULT_SCALE;
    origin = VIEW_MAP_DEFAULT_ORIGIN;
}

// Calculate the cell subscripts corresponding to the supplied location parameter,
// using the current size, scale, and origin of the display.
// This function assumes that origin is a  member variable of type Point,
// scale is a double value, and size is an integer for the number of rows/columns
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View_map::get_subscripts(int &ix, int &iy, Point location)
{
    // adjust with origin and scale
    Cartesian_vector subscripts = (location - origin) / scale;
    // truncate coordinates to integer after taking the floor
    // floor function will produce integer smaller than even for negative values,
    // so - 0.05 => -1., which will be outside the array.
    ix = int(floor(subscripts.delta_x));
    iy = int(floor(subscripts.delta_y));
    // if out of range, return false
    if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size))
        return false;
    else
        return true;
}
