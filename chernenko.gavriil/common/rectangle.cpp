#include <stdexcept>
#include "shapes/rectangle.hpp"
#include "shapes/base-types.hpp"

chernenko::Rectangle::Rectangle(double w, double h, point_t p):
  content_{w, h, p}
{
  if (w <= min_length_v || h <= min_length_v)
  {
    throw std::invalid_argument("Rectangle sides must be greater then 0");
  }
}

double chernenko::Rectangle::getArea() const noexcept
{
  return content_.width * content_.height;
}

chernenko::rectangle_t chernenko::Rectangle::getFrameRect() const noexcept
{
  return content_;
}

chernenko::Rectangle& chernenko::Rectangle::move(point_t to) noexcept
{
  content_.pos = to;
  return *this;
}

chernenko::Rectangle& chernenko::Rectangle::move(double dx, double dy) noexcept
{
  content_.pos.x += dx;
  content_.pos.y += dy;
  return *this;
}

chernenko::Rectangle& chernenko::Rectangle::scale(double multiplier)
{
  if (multiplier <= min_scale_multiplier_v)
  {
    throw std::invalid_argument("Scale multiplier cant be less or equal zero");
  }
  content_.width *= multiplier;
  content_.height *= multiplier;
  return *this;
}
