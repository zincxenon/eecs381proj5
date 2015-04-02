CC = g++
LD = g++

CFLAGS = -c -pedantic-errors -std=c++11 -Wall -g
LFLAGS = -pedantic -Wall

OBJS = p5_main.o Controller.o Cruise_ship.o Cruiser.o Geometry.o Island.o Model.o Navigation.o Ship.o Ship_factory.o Sim_object.o Tanker.o Track_base.o Utility.o View.o Views.o Warship.o
PROG = p5exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p5_main.o: p5_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p5_main.cpp

Controller.o: Controller.h Controller.cpp Model.h View.h Views.h Ship.h Island.h Ship_factory.h
	$(CC) $(CFLAGS) Controller.cpp

Cruise_ship.o: Cruise_ship.h Cruise_ship.cpp Ship.h Geometry.h Island.h
	$(CC) $(CFLAGS) Cruise_ship.cpp

Cruiser.o: Cruiser.h Cruiser.cpp Warship.h Ship.h Geometry.h
	$(CC) $(CFLAGS) Cruiser.cpp

Geometry.o: Geometry.h Geometry.cpp
	$(CC) $(CFLAGS) Geometry.cpp

Island.o: Island.h Island.cpp Model.h Geometry.h
	$(CC) $(CFLAGS) Island.cpp

Model.o: Model.h Model.cpp Ship.h Island.h View.h Geometry.h Ship_factory.h Utility.h
	$(CC) $(CFLAGS) Model.cpp

Navigation.o: Navigation.h Navigation.cpp Geometry.h
	$(CC) $(CFLAGS) Navigation.cpp

Ship.o: Ship.h Ship.cpp Model.h Geometry.h Navigation.h Track_base.h Utility.h Island.h
	$(CC) $(CFLAGS) Ship.cpp

Ship_factory.o: Ship_factory.h Ship_factory.cpp Geometry.h Ship.h Tanker.h Cruiser.h Cruise_ship.h
	$(CC) $(CFLAGS) Ship_factory.cpp

Sim_object.o: Sim_object.h Sim_object.cpp Utility.h
	$(CC) $(CFLAGS) Sim_object.cpp

Tanker.o: Tanker.h Tanker.cpp Ship.h Geometry.h Island.h
	$(CC) $(CFLAGS) Tanker.cpp

Track_base.o: Track_base.h Track_base.cpp Navigation.h
	$(CC) $(CFLAGS) Track_base.cpp

Utility.o: Utility.h Utility.cpp
	$(CC) $(CFLAGS) Utility.cpp

View.o: View.h View.cpp Geometry.h Utility.h
	$(CC) $(CFLAGS) View.cpp

Views.o: Views.h Views.cpp Geometry.h Navigation.h Utility.h
	$(CC) $(CFLAGS) Views.cpp

Warship.o: Warship.h Warship.cpp Ship.h Geometry.h Navigation.h
	$(CC) $(CFLAGS) Warship.cpp

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe

