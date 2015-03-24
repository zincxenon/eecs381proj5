#include "Ship_factory.h"
#include "Geometry.h"
#include "Ship.h"
#include "Tanker.h"
#include "Cruiser.h"
#include "Utility.h"
#include <string>
#include <memory>

using namespace std;

const string FACTORY_TANKER_TYPE = "Tanker";
const string FACTORY_CRUISER_TYPE = "Cruiser";

// may throw Error("Trying to create ship of unknown type!")
shared_ptr<Ship> create_ship(const string& name, const string& type, Point initial_position)
{
    if (type == FACTORY_TANKER_TYPE)
    {
        return shared_ptr<Ship>(make_shared<Tanker>(name, initial_position));
    } else if (type == FACTORY_CRUISER_TYPE)
    {
        return shared_ptr<Ship>(make_shared<Cruiser>(name, initial_position));
    }
    throw Error("Trying to create ship of unknown type!");
}