#include "Cruise_ship.h"
#include "Ship.h"
#include "Island.h"
#include "Model.h"
#include "Geometry.h"
#include "Utility.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

const double CRUISE_SHIP_INIT_FUEL = 500;
const double CRUISE_SHIP_MAX_SPEED = 15;
const double CRUISE_SHIP_FUEL_CONSUMPTION = 2;
const double CRUISE_SHIP_INIT_RESISTANCE = 0;

Cruise_ship::Cruise_ship(const std::string &name_, Point position_) :
        Ship(name_, position_, CRUISE_SHIP_INIT_FUEL, CRUISE_SHIP_MAX_SPEED,
                CRUISE_SHIP_FUEL_CONSUMPTION, CRUISE_SHIP_INIT_RESISTANCE),
        all_islands(), islands_left(), first_island(), cruise_speed(0), cruise_state(State_cruise_ship::OFF_CRUISE)
{
    Model::Island_map island_map = Model::get_Instance()->get_islands();
    for (auto&& island_pair : island_map) all_islands[island_pair.second->get_location()] = island_pair.second;
    cout << "Cruise ship " << get_name() << " constructed" << endl;
}

Cruise_ship::~Cruise_ship()
{
    cout << "Cruise ship " << get_name() << " destructed" << endl;
}

void update()
{

}

void describe() const
{

}

// checks if destination is an island, and if so, begins a cruise, or stops its cruise if the destination is not
void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed)
{
    if (cruise_state != State_cruise_ship::OFF_CRUISE) end_cruise();
    Ship::set_destination_position_and_speed(destination_position, speed);
    auto island_it = all_islands.find(destination_position);
    if (island_it == all_islands.end()) return;
    begin_cruise(*island_it);
}

// end cruise if necessary
void Cruise_ship::set_course_and_speed(double course, double speed)
{
    if (cruise_state != State_cruise_ship::OFF_CRUISE) end_cruise();
    Ship::set_course_and_speed(course, speed);
}
void Cruise_ship::stop()
{
    if (cruise_state != State_cruise_ship::OFF_CRUISE) end_cruise();
    Ship::stop();
}

void Cruise_ship::begin_cruise(std::shared_ptr<Island> island)
{
    islands_left(all_islands.begin(), all_islands.end());
    first_island = island;
    cruise_state = State_cruise_ship::TRAVELING_TO_ISLAND;
    cout << get_name() << " cruise will start and end at " << island->get_name() << endl;
}
void Cruise_ship::end_cruise()
{
    islands_left.clear();
    first_island.reset();
    cruise_state = State_cruise_ship::OFF_CRUISE;
    cout << get_name() << " canceling current cruise" << endl;
}