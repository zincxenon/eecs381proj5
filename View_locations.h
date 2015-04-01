#ifndef VIEW_LOCATIONS_H
#define VIEW_LOCATIONS_H

#include "Geometry.h"
#include "View.h"
#include <string>
#include <map>

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


#endif
