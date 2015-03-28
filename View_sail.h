#ifndef VIEW_SAILING_H
#define VIEW_SAILING_H

#include <string>
#include <map>
#include "Geometry.h"
#include "View.h"

/* *** Sailing View class ***
Shows data about ships in a short format
*/


class View_sail : public View {
public:
    View_sail();		// outputs constructor message
    ~View_sail();	// outputs destructor message

    // Save the supplied name and information for future use in a draw() call
    // If the name is already present,the new location replaces the previous one.
    void update_location_ship(const std::string& name, Point location) override;
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
        Point location;
        double course;
        double speed;
        double fuel;
    };

    std::map<std::string, Ship_data> ship_map;
};

#endif