#pragma once

#include <memory>
#include <vehicle.h>

class Factory
{
 public:
  virtual ~Factory() = default;
  virtual std::unique_ptr<Vehicle> createVehicle() const = 0;

  void propulseVehicle() const
  {
    auto vehicle = createVehicle();
    vehicle->propulse();
  }
};
