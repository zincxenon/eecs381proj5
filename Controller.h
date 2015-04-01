#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class Model;
class View;
class View_map;
class View_sail;
class View_bridge;
class Ship;
class Island;

/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/
class Controller {
public:	
	// output constructor message
	Controller();
	// output destructor message
	~Controller();

	// create View object, run the program by accepting user commands, then destroy View object
	void run();

private:
	std::vector<std::weak_ptr<View>> views;
	std::shared_ptr<View_map> view_map;
	std::shared_ptr<View_sail> view_sail;
	std::map<std::string, std::shared_ptr<View_bridge>> bridge_views;

	// helper functions
	int read_int();
	double read_double();
	double read_speed();
	std::shared_ptr<Ship> read_ship();
	std::shared_ptr<Island> read_island();

	// command functions
	typedef bool (Controller::*command_func)();
	// these functions return true if execution is to be ended, false otherwise
	bool quit();

	// view functions
	bool view_show();
	bool view_map_open();
	bool view_map_close();
	bool view_sail_open();
	bool view_sail_close();
	bool view_bridge_open();
	bool view_bridge_close();

	// view_map functions
	bool view_map_default();
	bool view_map_size();
	bool view_map_zoom();
	bool view_map_pan();

	// model functions
	bool model_status();
	bool model_go();
	bool model_create();

	// ship functions
	typedef void (Controller::*ship_func)(std::shared_ptr<Ship>);
	void ship_course(std::shared_ptr<Ship> ship);
	void ship_position(std::shared_ptr<Ship> ship);
	void ship_destination(std::shared_ptr<Ship> ship);
	void ship_load_at(std::shared_ptr<Ship> ship);
	void ship_unload_at(std::shared_ptr<Ship> ship);
	void ship_dock_at(std::shared_ptr<Ship> ship);
	void ship_attack(std::shared_ptr<Ship> ship);
	void ship_refuel(std::shared_ptr<Ship> ship);
	void ship_stop(std::shared_ptr<Ship> ship);
	void ship_stop_attack(std::shared_ptr<Ship> ship);

	std::map<std::string, command_func> command_func_map {
			{"quit", &Controller::quit},

			{"show", &Controller::view_show},
			{"open_map_view", &Controller::view_map_open},
			{"close_map_view", &Controller::view_map_close},
			{"open_sailing_view", &Controller::view_sail_open},
			{"close_sailing_view", &Controller::view_sail_close},
			{"open_bridge_view", &Controller::view_bridge_open},
			{"close_bridge_view", &Controller::view_bridge_close},

			{"default", &Controller::view_map_default},
			{"size", &Controller::view_map_size},
			{"zoom", &Controller::view_map_zoom},
			{"pan", &Controller::view_map_pan},

			{"status", &Controller::model_status},
			{"go", &Controller::model_go},
			{"create", &Controller::model_create}
	};

	std::map<std::string, ship_func> ship_func_map {
			{"course", &Controller::ship_course},
			{"position", &Controller::ship_position},
			{"destination", &Controller::ship_destination},
			{"load_at", &Controller::ship_load_at},
			{"unload_at", &Controller::ship_unload_at},
			{"dock_at", &Controller::ship_dock_at},
			{"attack", &Controller::ship_attack},
			{"refuel", &Controller::ship_refuel},
			{"stop", &Controller::ship_stop},
			{"stop_attack", &Controller::ship_stop_attack}
	};
};

#endif