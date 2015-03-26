#ifndef CRUISE_SHIP_H
#define CRUISE_SHIP_H

#include "Ship.h"
#include "Geometry.h"
#include <string>
#include <map>
#include <memory>

enum class State_cruise_ship {OFF_CRUISE, TRAVELING_TO_ISLAND, REFUELING, SIGHTSEEING};

class Cruise_ship : public Ship {
public:
    Cruise_ship(const std::string &name_, Point position_);

    ~Cruise_ship();

    void update() override;

    void describe() const override;

    // checks if destination is an island, and if so, begins a cruise, or stops its cruise if the destination is not
    void set_destination_position_and_speed(Point destination_position, double speed) override;

    // end cruise if necessary
    void set_course_and_speed(double course, double speed) override;
    void stop() override;

private:
    std::map<Point, std::shared_ptr<Island>> all_islands;        // all of the islands in the simulation
    std::vector<Island> islands_left;                           // islands left in this cruise

    std::shared_ptr<Island> first_island;   // the first island visited, to be returned to at the end of the cruise

    double cruise_speed;
    State_cruise_ship  cruise_state;

    void begin_cruise(std::shared_ptr<Island> island);
    void end_cruise();
};

#endif