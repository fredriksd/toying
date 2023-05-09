#pragma once

#include <vehicle.h>

class Truck : public Vehicle
{
public:
  void propulse() override;
};
