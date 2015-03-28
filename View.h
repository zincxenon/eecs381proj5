#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "Geometry.h"

/* *** View class ***
Represents the interface for a view to be displayed to the user
*/


class View {
public:
	~View();	// outputs destructor message

	// Save the supplied name and information for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	virtual void update_location_ship(const std::string& name, Point location) {}
	virtual void update_location_island(const std::string& name, Point location) {}
	virtual void update_course_and_speed(const std::string& name, double course, double speed) {}
	virtual void update_fuel(const std::string& name, double fuel) {}

	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove_ship(const std::string& name) {}

	// prints out the view
	virtual void draw() {}

	// Discard the saved information - drawing will show only a empty pattern
	virtual void clear() {}

	// modify the display parameters
	virtual void set_size(int size_) {}
	virtual void set_scale(double scale_) {}
	virtual void set_origin(Point origin_) {}

	// set the parameters to the default values
	virtual void set_defaults() {}

protected:
	View();		// outputs constructor message

};

#endif