#ifndef WARSHIP_H
#define WARSHIP_H

#include "Ship.h"
#include "Geometry.h"
#include "Navigation.h"
#include <string>
#include <memory>

/* Warship class
A Warship is a ship with firepower and range member variables, and some services for
protected classes to manage many of the details of warship behavior. This is an
abstract base class, so concrete classes derived from Warship must be declared.
*/

enum class State_warship {NOT_ATTACKING, ATTACKING};

class Warship : public Ship {
public:
	// output destructor message
	virtual ~Warship();
	
	// perform warship-specific behavior
	void update() override;

	// Warships will act on an attack and stop_attack command

	// will	throw Error("Cannot attack!") if not Afloat
	// will throw Error("Warship may not attack itself!")
    // if supplied target is the same as this Warship
	void attack(std::shared_ptr<Ship> target_ptr_) override;

	// will throw Error("Was not attacking!") if not Attacking
	void stop_attack() override;
	
	void describe() const override;

protected:
	// initialize, then output constructor message
	Warship(const std::string& name_, Point position_, double fuel_capacity_,
			double maximum_speed_, double fuel_consumption_, int resistance_,
			int firepower_, double maximum_range_);

	// return true if this Warship is in the attacking state
	bool is_attacking() const
	{
		return warship_state == State_warship::ATTACKING;
	}
	
	// fire at the current target
	void fire_at_target();
		
	// is the current target in range?
	bool target_in_range() const
	{
		if (target.expired()) return false;
		return cartesian_distance(get_location(), target.lock()->get_location()) < max_range;
	}

	// get the target
	std::shared_ptr<Ship> get_target() const
	{
		return target.lock();
	}

private:
	int firepower;
	double max_range;

	State_warship warship_state;

	std::weak_ptr<Ship> target;
};

#endif