#include <iostream>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <cmath>

#include "../common/shapes/base-types.hpp"
#include "../common/shapes/shape.hpp"
#include "../common/shapes/rectangle.hpp"
#include "../common/shapes/ellipse.hpp"
#include "../common/shapes/regular.hpp"

namespace chernenko
{
  const int bad_ec = 1;
  const int good_ec = 0;

  const std::size_t shapes_count_v = 3;
  const double rect_width_v = 3.4;
  const double rect_height_v = 3.4;
  const point_t rect_center_p{3.4, 4.3};
  const double ellipse_x_rad_v = 6.7;
  const double ellipse_y_rad_v = 2.7;
  const point_t ellipse_center_p{2.2, 8.0};
  const unsigned regular_angles_v = 6;
  const double regular_length_v = 9.0;
  const point_t regular_center_p{4.2, 5.2};

  std::ostream& operator<<(std::ostream& out, chernenko::point_t p);
  std::ostream& operator<<(std::ostream& out, chernenko::rectangle_t rect);

  double readDouble(std::istream& input);
  point_t readPoint(std::istream& input);
  point_t findLeftDownCorner(const rectangle_t& rect);
  point_t findRightUpCorner(const rectangle_t& rect);
  rectangle_t formRectangle(const point_t& l_corner, const point_t& r_corner);
  Shape& scaleAbout(Shape& s, double multiplier, point_t anchor);
  void scaleShapes(Shape* const* shapes, std::size_t k, point_t anchor, double scale_coef);
  double calcSumArea(const Shape* const* shapes, std::size_t k);
  rectangle_t formTotalFrameRect(const Shape* const* shapes, std::size_t k);
  void displayShapesSpecs(std::ostream& out, const Shape* const* shapes, std::size_t k);
  int process(std::istream& input, std::ostream& out, std::ostream& err_out);
}

int main()
{
  return chernenko::process(std::cin, std::cout, std::cerr);
}

std::ostream& chernenko::operator<<(std::ostream& out, chernenko::point_t p)
{
  return out << "point{ x=" << p.x << " y=" << p.y << " }";
}

std::ostream& chernenko::operator<<(std::ostream& out, chernenko::rectangle_t rect)
{
  return out << "rectangle{ pos=" << rect.pos << " height=" << rect.height << " width=" << rect.width << " }";
}

double chernenko::readDouble(std::istream& input)
{
  double res = 0;
  input >> res;
  if (!input)
  {
    throw std::runtime_error("Invalid input");
  }
  return res;
}

chernenko::point_t chernenko::readPoint(std::istream& input)
{
  return point_t{readDouble(input), readDouble(input)};
}

chernenko::point_t chernenko::findLeftDownCorner(const rectangle_t& rect)
{
  return point_t{rect.pos.x - rect.width / 2, rect.pos.y - rect.height / 2};
}

chernenko::point_t chernenko::findRightUpCorner(const rectangle_t& rect)
{
  return point_t{rect.pos.x + rect.width / 2, rect.pos.y + rect.height / 2};
}

chernenko::rectangle_t chernenko::formRectangle(const point_t& l_corner, const point_t& r_corner)
{
  const double w = r_corner.x - l_corner.x;
  const double h = r_corner.y - l_corner.y;
  return rectangle_t{w, h, {l_corner.x + w / 2, l_corner.y + h / 2}};
}

chernenko::Shape& chernenko::scaleAbout(Shape& s, double multiplier, point_t anchor)
{
  const point_t before = s.getFrameRect().pos;
  s.scale(multiplier);
  const point_t after = s.getFrameRect().pos;
  const point_t target{anchor.x + (before.x - anchor.x) * multiplier, anchor.y + (before.y - anchor.y) * multiplier};
  return s.move(target.x - after.x, target.y - after.y);
}

void chernenko::scaleShapes(Shape* const* shapes, std::size_t k, point_t anchor, double scale_coef)
{
  for (std::size_t i = 0; i < k; i++)
  {
    scaleAbout(*shapes[i], scale_coef, anchor);
  }
}

double chernenko::calcSumArea(const Shape* const* shapes, std::size_t k)
{
  double res = 0;
  for (std::size_t i = 0; i < k; i++)
  {
    res += shapes[i]->getArea();
  }
  return res;
}

chernenko::rectangle_t chernenko::formTotalFrameRect(const Shape* const* shapes, std::size_t k)
{
  if (shapes == nullptr || k == 0)
  {
    throw std::invalid_argument("Empty rects array");
  }
  rectangle_t cur_frame_rect = shapes[0]->getFrameRect();
  point_t l_corner = findLeftDownCorner(cur_frame_rect);
  point_t r_corner = findRightUpCorner(cur_frame_rect);
  for (std::size_t i = 1; i < k; i++)
  {
    cur_frame_rect = shapes[i]->getFrameRect();
    const point_t cur_l_corner = findLeftDownCorner(cur_frame_rect);
    const point_t cur_r_corner = findRightUpCorner(cur_frame_rect);
    l_corner.x = std::fmin(l_corner.x, cur_l_corner.x);
    l_corner.y = std::fmin(l_corner.y, cur_l_corner.y);
    r_corner.x = std::fmax(r_corner.x, cur_r_corner.x);
    r_corner.y = std::fmax(r_corner.y, cur_r_corner.y);
  }
  return formRectangle(l_corner, r_corner);
}

void chernenko::displayShapesSpecs(std::ostream& out, const Shape* const* shapes, std::size_t k)
{
  if (shapes == nullptr || k == 0)
  {
    throw std::invalid_argument("Empty shapes array");
  }
  out << "Shapes area: " << shapes[0]->getArea();
  for (std::size_t i = 1; i < k; i++)
  {
    out << ", " << shapes[i]->getArea();
  }
  out << std::endl << "Total area: " << calcSumArea(shapes, k) << std::endl;
  out << "Frame rects: " << shapes[0]->getFrameRect();
  for (std::size_t i = 1; i < k; i++)
  {
    out << ", " << shapes[i]->getFrameRect();
  }
  out << std::endl << "Total frame rect: " << formTotalFrameRect(shapes, k) << std::endl;
}

int chernenko::process(std::istream& input, std::ostream& out, std::ostream& err_out)
{
  try
  {
    Rectangle rectangle_s(rect_width_v, rect_height_v, rect_center_p);
    Ellipse ellipse_s(ellipse_x_rad_v, ellipse_y_rad_v, ellipse_center_p);
    Regular regular_s(regular_angles_v, regular_length_v, regular_center_p);
    Shape* shapes_arr[shapes_count_v]{&rectangle_s, &ellipse_s, &regular_s};
    out << "Input anchor point x and y" << std::endl;
    const point_t anchor_p = readPoint(input);
    out << "Input scale multiplier" << std::endl;
    const double scale_coef = readDouble(input);
    displayShapesSpecs(out << "Before scale: " << std::endl, shapes_arr, shapes_count_v);
    scaleShapes(shapes_arr, shapes_count_v, anchor_p, scale_coef);
    displayShapesSpecs(out << "After scale: " << std::endl, shapes_arr, shapes_count_v);
  }
  catch (const std::invalid_argument& e)
  {
    err_out << "Invalid argument: " << e.what() << std::endl;
    return bad_ec;
  }
  catch (const std::runtime_error& e)
  {
    err_out << "Runtime error: " << e.what() << std::endl;
    return bad_ec;
  }
  return good_ec;
}
