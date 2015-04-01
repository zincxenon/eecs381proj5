#include "Warship.h"
#include <iostream>
#include <cassert>

using namespace std;

// initialize, then output constructor message
Warship::Warship(const string& name_, Point position_, double fuel_capacity_,
        double maximum_speed_, double fuel_consumption_, int resistance_,
        int firepower_, double maximum_range_) :
        Ship(name_, position_, fuel_capacity_, maximum_speed_, fuel_consumption_, resistance_),
        firepower(firepower_), max_range(maximum_range_), warship_state(State_warship::NOT_ATTACKING), target()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Warship " << get_name() << " constructed" << endl;
}

// a pure virtual function to mark this as an abstract class,
// but defined anyway to output destructor message
Warship::~Warship()
{
    if (SHOW_CONSTRUCTOR_DESTRUCTOR_MSG) cout << "Warship " << get_name() << " destructed" << endl;
}

// perform warship-specific behavior
void Warship::update()
{
    Ship::update();
    if (warship_state == State_warship::NOT_ATTACKING) return;
    if (!is_afloat() || target.expired() || !target.lock()->is_afloat())
    {
        stop_attack();
    }
    else
    {
        cout << get_name() << " is attacking" << endl;
    }
}

// Warships will act on an attack and stop_attack command

// will	throw Error("Cannot attack!") if not Afloat
// will throw Error("Warship may not attack itself!")
// if supplied target is the same as this Warship
void Warship::attack(shared_ptr<Ship> target_ptr_)
{
    if (!is_afloat()) throw Error("Cannot attack!");
    if (target_ptr_ == shared_from_this()) throw Error("Warship may not attack itself!");
    shared_ptr<Ship> current_target = target.lock();
    if (target_ptr_ == current_target) throw Error("Already attacking this target!");
    target = target_ptr_;
    warship_state = State_warship::ATTACKING;
    cout << get_name() << " will attack " << target_ptr_->get_name() << endl;
}

// will throw Error("Was not attacking!") if not Attacking
void Warship::stop_attack()
{
    if (warship_state != State_warship::ATTACKING) throw Error("Was not attacking!");
    warship_state = State_warship::NOT_ATTACKING;
    target.reset();
    cout << get_name() << " stopping attack" << endl;
}

void Warship::describe() const
{
    Ship::describe();
    if (warship_state == State_warship::ATTACKING)
    {
        string target_output = "absent ship";
        if (!target.expired()) target_output = target.lock()->get_name();
        cout << "Attacking " << target_output << endl;
    }
}

// fire at the current target
void Warship::fire_at_target()
{
    cout << get_name() << " fires" << endl;
    assert(!target.expired());
    target.lock()->receive_hit(firepower, dynamic_pointer_cast<Ship, Sim_object>(shared_from_this()));
}