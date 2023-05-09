#pragma once

class Vehicle {
 public:
  virtual ~Vehicle() = default;

  virtual void propulse() = 0;
};
