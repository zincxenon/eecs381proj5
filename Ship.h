#ifndef SHIP_H
#define SHIP_H

#include "Sim_object.h"
#include "Track_base.h"
#include "Model.h"
#include "Geometry.h"
#include <string>
#include <memory>

class Island;

/***** Ship Class *****/
/* A Ship has a name, initial position, amount of fuel, and parameters that govern its movement.
The initial amount of fuel is equal to the supplied fuel capacity - a full fuel tank.
A Ship can be commanded to move to either a position or follow a course, or stop,
dock at or refuel at an Island. It consumes fuel while moving, and becomes immobile
if it runs out of fuel. It inherits the Sim_object interface to the rest of the system,
and the Track_base class provides the basic movement functionality, with the unit of time
corresponding to 1.0 for one "tick" - an hour of simulated time.

The update function updates the position and/or state of the ship.
The describe function outputs information about the ship state.
Accessors make the ship state available to either the public or to derived classes.
The is a "fat interface" for the capabilities of derived types of Ships. These
functions are implemented in this class to throw an Error exception.
*/

enum class State_ship {DOCKED, STOPPED, MOVING_TO_POSITION, MOVING_ON_COURSE, DEAD_IN_THE_WATER, SUNK};

class Ship : public Sim_object {
public:
    // output destructor message
    virtual ~Ship();

    /*** Readers ***/
    // return the current position
    Point get_location() const override {
        return track.get_position();
    }

    // Return true if ship can move (it is not dead in the water or in the process or sinking);
    bool can_move() const
    {
        return is_afloat() && ship_state != State_ship::DEAD_IN_THE_WATER;
    }

    // Return true if ship is moving;
    bool is_moving() const
    {
        return ship_state == State_ship::MOVING_ON_COURSE || ship_state == State_ship::MOVING_TO_POSITION;
    }

    // Return true if ship is docked;
    bool is_docked() const
    {
        return ship_state == State_ship::DOCKED;
    }

    // Return true if ship is afloat (not in process of sinking), false if not
    bool is_afloat() const
    {
        return ship_state != State_ship::SUNK;
    }

    // Return true if the ship is Stopped and the distance to the supplied island
    // is less than or equal to 0.1 nm
    bool can_dock(std::shared_ptr<Island> island_ptr) const;

    double get_fuel() const
    {
        return fuel;
    }

    double get_course() const
    {
        return track.get_course();
    }

    double get_speed() const
    {
        return track.get_speed();
    }

    /*** Interface to derived classes ***/
    // Update the state of the Ship
    void update() override;

    // output a description of current state to cout
    void describe() const override;

    void broadcast_current_state() override;

    /*** Command functions ***/
    // Start moving to a destination position at a speed
    // may throw Error("Ship cannot move!")
    // may throw Error("Ship cannot go that fast!")
    virtual void set_destination_position_and_speed(Point destination_position, double speed);

    // Start moving on a course and speed
    // may throw Error("Ship cannot move!")
    // may throw Error("Ship cannot go that fast!");
    virtual void set_course_and_speed(double course, double speed);

    // Stop moving
    // may throw Error("Ship cannot move!");
    virtual void stop();

    // dock at an Island - set our position = Island's position, go into Docked state
    // may throw Error("Can't dock!");
    virtual void dock(std::shared_ptr<Island> island_ptr);

    // Refuel - must already be docked at an island; fill takes as much as possible
    // may throw Error("Must be docked!");
    virtual void refuel();

    /*** Fat interface command functions ***/
    // These functions throw an Error exception for this class
    // will always throw Error("Cannot load at a destination!");
    virtual void set_load_destination(std::shared_ptr<Island>);

    // will always throw Error("Cannot unload at a destination!");
    virtual void set_unload_destination(std::shared_ptr<Island>);

    // will always throw Error("Cannot attack!");
    virtual void attack(std::shared_ptr<Ship> in_target_ptr);

    // will always throw Error("Cannot attack!");
    virtual void stop_attack();

    // interactions with other objects
    // receive a hit from an attacker
    virtual void receive_hit(int hit_force, std::shared_ptr<Ship> attacker_ptr);

    // disallow copy/move, construction or assignment
    Ship(const Ship&) = delete;
    Ship& operator=(const Ship&) = delete;

protected:
    // initialize, then output constructor message
    Ship(const std::string &name_, Point position_, double fuel_capacity_,
            double maximum_speed_, double fuel_consumption_, int resistance_);

    double get_maximum_speed() const
    {
        return max_speed;
    }

    // return pointer to the Island currently docked at, or nullptr if not docked
    std::shared_ptr<Island> get_docked_Island() const
    {
        return docked_at;
    }

private:
    double fuel;                        // Current amount of fuel
    double fuel_consumption;            // tons/nm required
    double fuel_capacity;               // Maximum fuel capacity
    Point destination;                    // Current destination if any
    Track_base track;                   // Track_base of this ship

    double max_speed;                   // Maximum speed
    int resistance;                  // Resistance of ship

    State_ship ship_state;                   // Current state of the ship
    std::shared_ptr<Island> docked_at;                     // If docked, the island the ship is docked at

    // Updates position, fuel, and movement_state, assuming 1 time unit (1 hr)
    void calculate_movement();

    // Check if the ship can move and the speed is within the max, and throws errors otherwise
    void check_movement_and_speed(double speed);

    // Prints the course and speed
    void print_course_and_speed() const;

};

#endif