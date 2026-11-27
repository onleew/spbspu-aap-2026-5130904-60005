#ifndef REGULAR_HPP
#define REGULAR_HPP

#include "base-types.hpp"
#include "shape.hpp"

namespace chernenko
{
  const unsigned min_angles_v = 2;

  class Regular final: public Shape
  {
  public:
    Regular(unsigned angles, double lenght, point_t center);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    Regular& move(point_t to) noexcept override;
    Regular& move(double dx, double dy) noexcept override;
    Regular& scale(double multiplier) override;

  private:
    unsigned angles_;
    double length_;
    point_t pos_;
  };
}

#endif
