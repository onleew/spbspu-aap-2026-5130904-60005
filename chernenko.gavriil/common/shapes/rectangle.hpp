#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "base-types.hpp"
#include "shape.hpp"

namespace chernenko
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(double w, double h, point_t p);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    Rectangle& move(point_t to) noexcept override;
    Rectangle& move(double dx, double dy) noexcept override;
    Rectangle& scale(double multiplier) override;
  private:
    rectangle_t content_;
  };
}

#endif
