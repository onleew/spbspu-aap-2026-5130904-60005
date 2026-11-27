#include <cmath>
#include <stdexcept>
#include "shapes/regular.hpp"
#include "shapes/base-types.hpp"

chernenko::Regular::Regular(unsigned angles, double lenght, point_t center):
  angles_(angles),
  length_(lenght),
  pos_(center)
{
  if (angles_ <= min_angles_v || length_ <= min_length_v)
  {
    throw std::invalid_argument("Regular must have greater then 2 angles and great then 0 lenght");
  }
}

double chernenko::Regular::getArea() const noexcept
{
  return (static_cast< double >(angles_) * length_ * length_) / (4 * std::tan(pi / static_cast< double >(angles_)));
}

chernenko::rectangle_t chernenko::Regular::getFrameRect() const noexcept
{
  const double step = 2.0 * pi / static_cast< double >(angles_);
  const double radius = length_ / (2.0 * std::sin(pi / static_cast< double >(angles_)));
  double min_x = pos_.x;
  double max_x = pos_.x;
  double min_y = pos_.y;
  double max_y = pos_.y;
  for (unsigned i = 0; i < angles_; i++)
  {
    const double angle = (pi * 0.5) + (step * static_cast< double >(i));
    const double x = pos_.x + (radius * std::cos(angle));
    const double y = pos_.y + (radius * std::sin(angle));
    min_x = std::fmin(min_x, x);
    max_x = std::fmax(max_x, x);
    min_y = std::fmin(min_y, y);
    max_y = std::fmax(max_y, y);
  }
  const point_t frame_center{(min_x + max_x) * 0.5, (min_y + max_y) * 0.5};
  return rectangle_t{max_x - min_x, max_y - min_y, frame_center};
}

chernenko::Regular& chernenko::Regular::move(point_t to) noexcept
{
  pos_ = to;
  return *this;
}

chernenko::Regular& chernenko::Regular::move(double dx, double dy) noexcept
{
  pos_.x += dx;
  pos_.y += dy;
  return *this;
}

chernenko::Regular& chernenko::Regular::scale(double multiplier)
{
  if (multiplier <= min_scale_multiplier_v)
  {
    throw std::invalid_argument("Scale multiplier cant be less or equal zero");
  }
  length_ *= multiplier;
  return *this;
}
