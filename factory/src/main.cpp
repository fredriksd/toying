#include <memory>
#include <boat.h>
#include <boat_factory.h>

std::unique_ptr<Vehicle> produceVehicle()
{
  BoatFactory bf{};
  return bf.createVehicle();
}

void doThing(const Factory& factory)
{
  factory.propulseVehicle();
}

int main()
{
  const auto boat_factory = BoatFactory{};
  doThing(boat_factory);
}
