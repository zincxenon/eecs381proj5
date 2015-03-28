#ifndef SIM_OBJECT_H
#define SIM_OBJECT_H
/* This class provides the interface for all of simulation objects. It also stores the
object's name, and has pure virtual accessor functions for the object's position
and other information. */

#include <string>
#include <memory>

struct Point;

class Sim_object : public std::enable_shared_from_this<Sim_object> {
public:

    virtual ~Sim_object();
	
	const std::string& get_name() const
		{return name;}
    
	// ask model to notify views of current state
    virtual void broadcast_current_state() {}

	/* Interface for derived classes */
	virtual Point get_location() const = 0;
	virtual void describe() const = 0;
	virtual void update() = 0;

protected:
	Sim_object(const std::string& name_);
	
private:
	std::string name;
};


#endif
