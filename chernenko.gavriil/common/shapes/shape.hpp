#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "base-types.hpp"

namespace chernenko
{
  class Shape
  {
  public:
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual Shape& move(point_t to) noexcept = 0;
    virtual Shape& move(double dx, double dy) noexcept = 0;
    virtual Shape& scale(double multiplier) = 0;
  };
}

#endif
