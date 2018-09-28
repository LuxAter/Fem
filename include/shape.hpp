#ifndef FEM_SHAPE_HPP_
#define FEM_SHAPE_HPP_

namespace fem {
namespace shape {
  struct Point {
   public:
    Point();
    Point(const double& x, const double& y);
    double x, y;
  };
  struct Circle {
   public:
    Circle();
    Circle(const double& x, const double& y, const double& r);
    Circle(const Point& c, const double& r);
    Point Center();
    bool In(double px, double py);
    double x, y, r, r2;
  };
  struct Triangle {
   public:
    Triangle();
    Triangle(const double& x0, const double& y0, const double& x1,
             const double& y1, const double& x2, const double& y2);
    Triangle(const Point& p0, const Point& p1, const Point& p2);
    Circle CircumCircle();
    Point Center();
    bool InCircumCircle(double px, double py);
    bool InCircumCircle(const Point& pt);
    double x0, y0, x1, y1, x2, y2;
    double xc, yc;
    double cc_x, cc_y, cc_r, cc_r2;

   private:
  };
}  // namespace shape
}  // namespace fem

#endif  // FEM_SHAPE_HPP_
