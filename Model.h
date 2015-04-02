#ifndef MODEL_H
#define MODEL_H

#include "Geometry.h"
#include "Utility.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

class Sim_object;
class Ship;
class Island;
class View;

/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/

class Model {
public:
	typedef std::shared_ptr<Island> Island_ptr;
	typedef std::shared_ptr<Ship> Ship_ptr;
	typedef std::shared_ptr<Sim_object> Sim_object_ptr;

	typedef std::map<std::string, Island_ptr> Island_map;

	static Model* get_Instance();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either ship or island?
    // either the identical name, or identical in first two characters counts as in-use
	bool is_name_in_use(const std::string& name) const
    {
        return objects.find(name) != objects.end();
    }

	// is there such an island?
	bool is_island_present(const std::string& name) const
    {
        return islands.find(name) != islands.end();
    }
	// will throw Error("Island not found!") if no island of that name
	Island_ptr get_island_ptr(const std::string& name) const;
	// returns a vector of ptrs to every island
	Island_map get_islands() const
	{
		return islands;
	};

	// is there such an ship?
	bool is_ship_present(const std::string& name) const
    {
        return ships.find(name) != ships.end();
    }
	// add a new ship to the list, and update the view
	void add_ship(Ship_ptr);
	// will throw Error("Ship not found!") if no ship of that name
	Ship_ptr get_ship_ptr(const std::string& name) const;
	void remove_ship(Ship_ptr ship);
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();
    
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects' locations (or other state information).
	void attach(std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View>);

	// notify the views about a ship's location
	void notify_location_ship(const std::string &name, Point location);
	// notify the views about a island's location
	void notify_location_island(const std::string &name, Point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
	// notify the views that a ship has changed fuel
	void notify_fuel(const std::string& name, double fuel);
	// notify the views that a ship has changed course and speed
	void notify_course_speed(const std::string& name, double course, double speed);

	// disallow copy/move construction or assignment
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

private:
	// create the initial objects, output constructor message
	Model();
	static Model* model;

	// destroy all objects, output destructor message
	~Model();

	int time;		// the simulated time

	typedef std::map<std::string, Ship_ptr> Ship_map;
	struct title_substring_compare
	{
		bool operator()(const std::string& first, const std::string& second) const
		{
			return first.substr(0, SHORTEN_NAME_LENGTH) < second.substr(0, SHORTEN_NAME_LENGTH);
		}
	};
	typedef std::map<std::string, Sim_object_ptr, title_substring_compare> Sim_object_map;

    Island_map islands;
	Ship_map ships;
	Sim_object_map objects;

    std::vector<std::shared_ptr<View>> views;
};

#endif