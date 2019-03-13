#include "vector.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "logger.hpp"

fem::math::Vector::Vector() : vals_() {}
fem::math::Vector::Vector(unsigned long n) : vals_(n, 0.0) {}
fem::math::Vector::Vector(unsigned long n, double v) : vals_(n, v) {}
fem::math::Vector::Vector(const Vector& copy) : vals_(copy.vals_) {}

void fem::math::save_vec_to_file(const std::string& file_name,
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
fem::math::Vector fem::math::load_vec_from_file(const std::string& file_name) {
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

fem::math::Vector fem::math::operator+(const Vector& lhs, const Vector& rhs){
  Vector vec(std::min(lhs.size(), rhs.size()));
  for(unsigned long i = 0; i < vec.size(); ++i){

  }
  return vec;
}
fem::math::Vector fem::math::operator-(const Vector& lhs, const Vector& rhs){}
fem::math::Vector fem::math::operator*(const Vector& lhs, const double& rhs){}
