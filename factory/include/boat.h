#pragma once

#include <vehicle.h>

class Boat : public Vehicle
{
public:
  void propulse() override;
};
