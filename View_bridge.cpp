#include "View_bridge.h"
#include "Navigation.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cmath>

using namespace std;

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
    if (bearing < -1 * VIEW_BRIDGE_HALF)
    {
        bearing += VIEW_BRIDGE_FULL;
    }
    if (bearing > VIEW_BRIDGE_HALF)
    {
        bearing -= VIEW_BRIDGE_HALF;
    }
    x = int(floor((bearing - VIEW_BRIDGE_MIN_SHOW) / VIEW_BRIDGE_SCALE));
    if (x < 0 || x >= VIEW_BRIDGE_MAP_WIDTH) return false;
    return true;
}