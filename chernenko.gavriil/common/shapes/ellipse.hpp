#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "base-types.hpp"
#include "shape.hpp"

namespace chernenko
{
  class Ellipse final: public Shape
  {
  public:
    Ellipse(double x_rad, double y_rad, point_t center);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    Ellipse& move(point_t to) noexcept override;
    Ellipse& move(double dx, double dy) noexcept override;
    Ellipse& scale(double multiplier) override;

  private:
    double x_radius_;
    double y_radius_;
    point_t pos_;
  };
}

#endif
