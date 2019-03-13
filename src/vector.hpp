#ifndef FEM_MATH_VECTOR_HPP_
#define FEM_MATH_VECTOR_HPP_

#include <string>
#include <vector>

namespace fem {
namespace math {
  class Vector {
   public:
    Vector();
    explicit Vector(unsigned long n);
    Vector(unsigned long n, double v);
    Vector(const Vector& copy);

    inline unsigned long size() const noexcept { return vals_.size(); }
    double& operator[](unsigned long i) { return vals_.at(i); }
    double operator[](unsigned long i) const { return vals_.at(i); }
    double& operator()(unsigned long i) { return vals_.at(i); }
    double operator()(unsigned long i) const { return vals_.at(i); }
    double at(unsigned long i) const { return vals_.at(i); }
    void set(unsigned long i, const double& val) { vals_.at(i) = val; }

    void clear() { vals_.clear(); }

    std::vector<double>* get_vals() { return &vals_; }
    const std::vector<double>* get_vals() const { return &vals_; }

   private:
    std::vector<double> vals_;
  };

  void save_vec_to_file(const std::string& file_name, const Vector& vec);
  Vector load_vec_from_file(const std::string& file_name);

  Vector operator+(const Vector& lhs, const Vector& rhs);
  Vector operator-(const Vector& lhs, const Vector& rhs);
  Vector operator*(const Vector& lhs, const double& rhs);

  double dot(const Vector& lhs, const Vector& rhs);
  double norm(const Vector& lhs);
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_VECTOR_HPP_
