#include <memory>

#include <boat.h>
#include <vehicle.h>
#include <factory.h>

class BoatFactory : public Factory
{
 public:
  std::unique_ptr<Vehicle> createVehicle() const override
  {
    return std::make_unique<Boat>();
  }
};
