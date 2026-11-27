#include <stdexcept>
#include "shapes/ellipse.hpp"
#include "shapes/base-types.hpp"

chernenko::Ellipse::Ellipse(double x_rad, double y_rad, point_t center):
  x_radius_(x_rad),
  y_radius_(y_rad),
  pos_(center)
{
  if (x_radius_ <= min_length_v || y_radius_ <= min_length_v)
  {
    throw std::invalid_argument("Ellipse's radiuses must be greater then 0");
  }
}

double chernenko::Ellipse::getArea() const noexcept
{
  return pi * x_radius_ * y_radius_;
}

chernenko::rectangle_t chernenko::Ellipse::getFrameRect() const noexcept
{
  return rectangle_t{x_radius_ * 2, y_radius_ * 2, pos_};
}

chernenko::Ellipse& chernenko::Ellipse::move(point_t to) noexcept
{
  pos_ = to;
  return *this;
}

chernenko::Ellipse& chernenko::Ellipse::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
  return *this;
}

chernenko::Ellipse& chernenko::Ellipse::scale(double multiplier)
{
  if (multiplier <= min_scale_multiplier_v)
  {
    throw std::invalid_argument("Scale multiplier cant be less or equal zero");
  }
  x_radius_ *= multiplier;
  y_radius_ *= multiplier;
  return *this;
}
