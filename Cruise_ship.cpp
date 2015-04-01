#include "Cruise_ship.h"
#include "Island.h"
#include <cassert>
#include <algorithm>
#include <iostream>

using namespace std;

const double CRUISE_SHIP_INIT_FUEL = 500;
const double CRUISE_SHIP_MAX_SPEED = 15;
const double CRUISE_SHIP_FUEL_CONSUMPTION = 2;
const double CRUISE_SHIP_INIT_RESISTANCE = 0;

Cruise_ship::Cruise_ship(const std::string &name_, Point position_) :
        Ship(name_, position_, CRUISE_SHIP_INIT_FUEL, CRUISE_SHIP_MAX_SPEED,
                CRUISE_SHIP_FUEL_CONSUMPTION, CRUISE_SHIP_INIT_RESISTANCE),
        cruise_speed(0), cruise_state(State_cruise_ship::OFF_CRUISE)
{
    Model::Island_map island_map = Model::get_Instance()->get_islands();
    for (auto&& island_pair : island_map) all_islands[island_pair.second->get_location()] = island_pair.second;
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Cruise ship " << get_name() << " constructed" << endl;
}

Cruise_ship::~Cruise_ship()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Cruise ship " << get_name() << " destructed" << endl;
}

void Cruise_ship::update()
{
    Ship::update();

    switch (cruise_state)
    {
        case State_cruise_ship::OFF_CRUISE:
            return;
        case State_cruise_ship::TRAVELING_TO_ISLAND:
            if (!can_dock(target_island)) return;
            dock(target_island);
            if (target_island == first_island && islands_left.empty())
            {
                cout << get_name() << " cruise is over at " << first_island->get_name() << endl;
                end_cruise();
                return;
            }
            cruise_state = State_cruise_ship::REFUELING;
            return;
        case State_cruise_ship::REFUELING:
            refuel();
            cruise_state = State_cruise_ship::SIGHTSEEING;
            // scope for declaring variables
            {
                auto island_it = lower_bound(islands_left.begin(), islands_left.end(),
                        target_island, island_name_compare);
                assert((*island_it)->get_name() == target_island->get_name() && island_it != islands_left.end());
                islands_left.erase(island_it);
            }
            target_island.reset();
            return;
        case State_cruise_ship::SIGHTSEEING:
            // do nothing while passengers see the sights
            cruise_state = State_cruise_ship::READY_TO_DEPART;
            return;
        case State_cruise_ship::READY_TO_DEPART:
            // scope for declaring variables
            {
                double min_dist = -1;
                target_island = first_island;
                for (auto &&island : islands_left) {
                    double dist = cartesian_distance(get_location(), island->get_location());
                    // note: because we are iterating through islands_left in order,
                    // if there is a tie the first alphabetical island will be chosen
                    if (min_dist == -1 || dist < min_dist) {
                        min_dist = dist;
                        target_island = island;
                    }
                }
            }
            // crash if for some reason the function call fails
            try {Ship::set_destination_position_and_speed(target_island->get_location(), cruise_speed);}
            catch (...) {assert(false);}
            cout << get_name() << " will visit " << target_island->get_name() << endl;
            cruise_state = State_cruise_ship::TRAVELING_TO_ISLAND;
            return;
    }
}

void Cruise_ship::describe() const
{
    cout << "\nCruise_ship ";
    Ship::describe();
    switch (cruise_state)
    {
        case State_cruise_ship::OFF_CRUISE:
            return;
        case State_cruise_ship::TRAVELING_TO_ISLAND:
            cout << "On cruise to " << target_island->get_name() << endl;
            return;
        default:
            cout << "Waiting during cruise at " << get_docked_Island()->get_name() << endl;
            return;
    }
}

// checks if destination is an island, and if so, begins a cruise, or stops its cruise if the destination is not
void Cruise_ship::set_destination_position_and_speed(Point destination_position, double speed)
{
    check_and_cancel_cruise();
    Ship::set_destination_position_and_speed(destination_position, speed);
    auto island_it = all_islands.find(destination_position);
    if (island_it == all_islands.end()) return;
    begin_cruise(speed, (*island_it).second);
}

// end cruise if necessary
void Cruise_ship::set_course_and_speed(double course, double speed)
{
    check_and_cancel_cruise();
    Ship::set_course_and_speed(course, speed);
}
void Cruise_ship::stop()
{
    check_and_cancel_cruise();
    Ship::stop();
}

void Cruise_ship::begin_cruise(double speed, shared_ptr<Island> island)
{
    //islands_left.push_back(pair.second);
    for (auto&& island : all_islands)
    {
        auto island_it = lower_bound(islands_left.begin(), islands_left.end(), island.second, island_name_compare);
        islands_left.insert(island_it, island.second);
    }
    //for_each(all_islands.begin(), all_islands.end(), [this](pair<Point, shared_ptr<Island>> pair){});
    // islands_left will be in order by name because all_islands is in order by name
    first_island = island;
    target_island = island;
    cruise_state = State_cruise_ship::TRAVELING_TO_ISLAND;
    cruise_speed = speed;
    cout << get_name() << " will visit " << island->get_name() << endl;
    cout << get_name() << " cruise will start and end at " << island->get_name() << endl;
}
void Cruise_ship::end_cruise()
{
    islands_left.clear();
    first_island.reset();
    target_island.reset();
    cruise_state = State_cruise_ship::OFF_CRUISE;
    cruise_speed = 0;
}
void Cruise_ship::check_and_cancel_cruise()
{
    if (cruise_state != State_cruise_ship::OFF_CRUISE)
    {
        end_cruise();
        cout << get_name() << " canceling current cruise" << endl;
    }
}