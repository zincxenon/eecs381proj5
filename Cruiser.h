#ifndef CRUISER_H
#define CRUISER_H

#include "Warship.h"
#include "Ship.h"
#include "Geometry.h"
#include <string>

/*
A Cruiser is a Warship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm, 
resistance 6, firepower 3, maximum attacking range 15
*/

class Cruiser : public Warship {
public:
	// initialize, then output constructor message
	Cruiser(const std::string &name_, Point position_);

	// output destructor message
	~Cruiser();

	void update() override;

	void describe() const override;

	void receive_hit(int hit_force, Ship *attacker_ptr) override;

};

#endif