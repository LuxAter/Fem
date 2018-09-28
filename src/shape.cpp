#include "shape.hpp"

#include <cmath>

fem::shape::Point::Point() {}
fem::shape::Point::Point(const double& x, const double& y) : x(x), y(y) {}

fem::shape::Circle::Circle() {}
fem::shape::Circle::Circle(const double& x, const double& y, const double& r)
    : x(x), y(y), r(r), r2(r * r) {}
fem::shape::Circle::Circle(const Point& c, const double& r)
    : x(c.x), y(c.y), r(r), r2(r * r) {}

fem::shape::Point fem::shape::Circle::Center() { return Point(x, y); }

bool fem::shape::Circle::In(double px, double py) {
  return pow(px - x, 2) + pow(py - y, 2) > r2;
}

fem::shape::Triangle::Triangle() {}
fem::shape::Triangle::Triangle(const double& x0_, const double& y0_,
                               const double& x1_, const double& y1_,
                               const double& x2_, const double& y2_)
    : x0(x0_),
      y0(y0_),
      x1(x1_),
      y1(y1_),
      x2(x2_),
      y2(y2_),
      xc((x0 + x1 + x2) / 3),
      yc((y0 + y1 + y2) / 3) {
  double a_0 = x0 * x0 + y0 * y0;
  double a_1 = x1 * x1 + y1 * y1;
  double a_2 = x2 * x2 + y2 * y2;
  double y_0 = y1 - y2;
  double y_1 = y0 - y2;
  double y_2 = y0 - y1;
  double bx = -a_0 * y_0 + a_1 * y_1 - a_2 * y_2;
  double by = a_0 * (x1 - x2) - a_1 * (x0 - x2) + a_2 * (x0 - x1);
  double a = x0 * y_0 - x1 * y_1 + x2 * y_2;
  double c = -a_0 * (x1 * y2 - x2 * y1) + a_1 * (x0 * y2 - x2 * y0) -
             a_2 * (x0 * y1 - x1 * y0);
  cc_x = -bx / (2 * a);
  cc_y = -by / (2 * a);
  cc_r = sqrt(bx * bx + by * by - 4 * a * c) / (2 * fabs(a));
  cc_r2 = cc_r * cc_r;
}
fem::shape::Triangle::Triangle(const Point& p0, const Point& p1,
                               const Point& p2)
    : x0(p0.x),
      y0(p0.y),
      x1(p1.x),
      y1(p1.y),
      x2(p2.x),
      y2(p2.y),
      xc((p0.x + p1.x + p2.x) / 3),
      yc((p0.y + p1.y + p2.y) / 3) {
  double a_0 = x0 * x0 + y0 * y0;
  double a_1 = x1 * x1 + y1 * y1;
  double a_2 = x2 * x2 + y2 * y2;
  double y_0 = y1 - y2;
  double y_1 = y0 - y2;
  double y_2 = y0 - y1;
  double bx = -a_0 * y_0 + a_1 * y_1 - a_2 * y_2;
  double by = a_0 * (x1 - x2) - a_1 * (x0 - x2) + a_2 * (x0 - x1);
  double a = x0 * y_0 - x1 * y_1 + x2 * y_2;
  double c = -a_0 * (x1 * y2 - x2 * y1) + a_1 * (x0 * y2 - x2 * y0) -
             a_2 * (x0 * y1 - x1 * y0);
  cc_x = -bx / (2 * a);
  cc_y = -by / (2 * a);
  cc_r = sqrt(bx * bx + by * by - 4 * a * c) / (2 * fabs(a));
  cc_r2 = cc_r * cc_r;
}
fem::shape::Circle fem::shape::Triangle::CircumCircle() {
  return Circle(cc_x, cc_y, cc_r);
}
fem::shape::Point fem::shape::Triangle::Center() { return Point(xc, yc); }
bool fem::shape::Triangle::InCircumCircle(double px, double py) {
  return pow(px - cc_x, 2) + pow(py - cc_y, 2) > cc_r2;
}
bool fem::shape::Triangle::InCircumCircle(const Point& pt) {
  return (pt.x - cc_x) * (pt.x - cc_x) + (pt.y - cc_y) * (pt.y - cc_y);
}
