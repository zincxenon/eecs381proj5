#ifndef VIEW_SAILING_H
#define VIEW_SAILING_H

#include "Geometry.h"
#include "View.h"
#include <string>
#include <map>
#include <cassert>

/* *** Sailing View class ***
Shows data about ships in a short format
*/

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