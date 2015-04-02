#ifndef VIEWS_H
#define VIEWS_H

#include "View.h"
#include "Geometry.h"
#include <cassert>
#include <string>
#include <map>

/* bridge view class */
class View_bridge : public View_locations {
public:
    View_bridge(const std::string& name);
    ~View_bridge();

    void update_course_and_speed(const std::string& name, double course, double speed) override;

    void update_remove_ship(const std::string& name) override;

    // prints out the view
    void draw() override;

private:
    std::string target;
    Point target_location;
    double target_course;
    bool target_sunk;

    bool get_heading(int& x, Point location);
};

/* this subclass is used by Views which get their data from a map of object name to location data */
class View_locations : public View {
public:
    ~View_locations();	// outputs destructor message

    // Save the supplied name and information for future use in a draw() call
    // If the name is already present,the new location replaces the previous one.
    void update_location_ship(const std::string& name, Point location) override;
    void update_location_island(const std::string& name, Point location) override;

    // Remove the name and its location; no error if the name is not present.
    void update_remove_ship(const std::string& name) override;

    // Discard the saved information - drawing will show only a empty pattern
    void clear() override
    {
        object_data.clear();
    }

protected:
    View_locations();
    std::map<std::string, Point> object_data;
};

/* *** View_map class ***
The View_map class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

Usage:
1. Call the update_location function with the name and position of each object
to be plotted. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location.

2. Call the update_remove function with the name of any object that should
no longer be plotted. This must be done *after* any call to update_location that
has the same object name since update_location will add any object name supplied.

3. Call the draw function to print out the map.

4. As needed, change the origin, scale, or displayed size of the map
with the appropriate functions. Since the view "remembers" the previously updated
information, the draw function will print out a map showing the previous objects
using the new settings.
*/
class View_map : public View_locations {
public:
    // default constructor sets the default size, scale, and origin, outputs constructor message
    View_map();
    ~View_map();	// outputs destructor message

    // prints out the current map
    void draw() override;

    // modify the display parameters
    // if the size is out of bounds will throw Error("New map size is too big!")
    // or Error("New map size is too small!")
    void set_size(int size_);

    // If scale is not positive, will throw Error("New map scale must be positive!");
    void set_scale(double scale_);

    // any values are legal for the origin
    void set_origin(Point origin_)
    {
        origin = origin_;
    }

    // set the parameters to the default values
    void set_defaults();

private:
    int size;			// current size of the display
    double scale;		// distance per cell of the display
    Point origin;		// coordinates of the lower-left-hand corner

    // Calculate the cell subscripts corresponding to the location parameter, using the
    // current size, scale, and origin of the display.
    // Return true if the location is within the map, false if not
    bool get_subscripts(int &ix, int &iy, Point location);

};

/* sailing view class */
class View_sail : public View {
public:
    View_sail();		// outputs constructor message
    ~View_sail();	// outputs destructor message

    // Save the supplied name and information for future use in a draw() call
    void update_course_and_speed(const std::string& name, double course, double speed) override;
    void update_fuel(const std::string& name, double fuel) override;

    // Remove the name and its location; no error if the name is not present.
    void update_remove_ship(const std::string& name) override;

    // prints out the current map
    void draw() override;

    // Discard the saved information - drawing will show only a empty pattern
    void clear() override;

private:
    struct Ship_data
    {
    public:
        Ship_data() : course_speed_defined(false), fuel_defined(false) {};

        double get_course()
        {
            if (course_speed_defined) return course;
            assert(false);
        }
        double get_speed()
        {
            if (course_speed_defined) return speed;
            assert(false);
        }
        double get_fuel()
        {
            if (fuel_defined) return fuel;
            assert(false);
        }
        void set_course_speed(double course_, double speed_)
        {
            course = course_;
            speed = speed_;
            course_speed_defined = true;
        }
        void set_fuel(double fuel_)
        {
            fuel = fuel_;
            fuel_defined = true;
        }
    private:
        double course;
        double speed;
        bool course_speed_defined;
        double fuel;
        bool fuel_defined;
    };

    std::map<std::string, Ship_data> ship_map;
};

#endif