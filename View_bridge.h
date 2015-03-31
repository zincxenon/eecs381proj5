#ifndef VIEW_BRIDGE_H
#define VIEW_BRIDGE_H

#include <string>
#include <map>
#include "Geometry.h"
#include "View_locations.h"

class View_bridge : public View_locations {
public:
    View_bridge(std::string& name);
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


#endif