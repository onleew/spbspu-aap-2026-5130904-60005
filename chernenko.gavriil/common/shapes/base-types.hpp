#ifndef BASE_TYPES_HPP
#define BASE_TYPES_HPP

namespace chernenko
{
  const double pi = 3.14159265358979323846;
  const double min_length_v = 0.0;
  const double min_scale_multiplier_v = 0.0;

  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };
}

#endif
