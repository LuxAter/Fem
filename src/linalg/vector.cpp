#include "vector.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "../logger.hpp"
#include "../print.hpp"

arta::math::Vector::Vector() : vals_() {}
arta::math::Vector::Vector(unsigned long n) : vals_(n, 0.0) {}
arta::math::Vector::Vector(unsigned long n, double v) : vals_(n, v) {}
arta::math::Vector::Vector(const Vector& copy) : vals_(copy.vals_) {}

std::string arta::math::Vector::dump() const {
  std::string str;
  for (int i = 0; i < vals_.size(); ++i) {
    str += arta::fmt_val(vals_[i]);
    if (i < vals_.size() - 1) {
      str += " ";
    }
  }
  return str;
}

void arta::math::save_vec_to_file(const std::string& file_name,
                                  const Vector& vec) {
  FILE* out = fopen(file_name.c_str(), "w");
  if (!out) {
    log::warning("Failed to open file \"%s\"", file_name.c_str());
    return;
  }
  fprintf(out, "%lu\n", vec.size());
  const std::vector<double>* vals = vec.get_vals();
  for (unsigned long i = 0; i < vals->size(); ++i) {
    fprintf(out, "%lf ", vals->at(i));
  }
  fclose(out);
}
arta::math::Vector arta::math::load_vec_from_file(
    const std::string& file_name) {
  FILE* src = fopen(file_name.c_str(), "r");
  if (!src) {
    log::warning("Failed to open file \"%s\"", file_name.c_str());
    return Vector();
  }
  unsigned long size;
  fscanf(src, "%lu", &size);
  Vector vec(size);
  for (unsigned long i = 0; i < size; ++i) {
    fscanf(src, "%lf", &vec[i]);
  }
  fclose(src);
  return vec;
}

arta::math::Vector arta::math::operator+(const Vector& lhs, const Vector& rhs) {
  Vector vec(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < vec.size(); ++i) {
    vec.set(i, lhs.at(i) + rhs.at(i));
  }
  return vec;
}
arta::math::Vector arta::math::operator-(const Vector& lhs, const Vector& rhs) {
  Vector vec(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < vec.size(); ++i) {
    vec.set(i, lhs.at(i) - rhs.at(i));
  }
  return vec;
}
arta::math::Vector arta::math::operator*(const Vector& lhs, const double& rhs) {
  Vector vec(lhs.size());
  for (unsigned long i = 0; i < vec.size(); ++i) {
    vec.set(i, lhs.at(i) * rhs);
  }
  return vec;
}

double arta::math::dot(const Vector& lhs, const Vector& rhs) {
  double val = 0.0;
  for (unsigned long i = 0; i < lhs.size() && i < rhs.size(); ++i) {
    val += (lhs.at(i) * rhs.at(i));
  }
  return val;
}

double arta::math::norm(const Vector& lhs) {
  double val = 0.0;
  for (unsigned long i = 0; i < lhs.size(); ++i) {
    val += (lhs.at(i) * lhs.at(i));
  }
  return std::sqrt(val);
}
