#include "matrix.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "../logger.hpp"
#include "../print.hpp"
#include "vector.hpp"

arta::linalg::Matrix::Matrix() : size_(0) {}
arta::linalg::Matrix::Matrix(unsigned long n) : size_(n), row_ptr_(n + 1, 0) {}
arta::linalg::Matrix::Matrix(unsigned long n, const double& v)
    : size_(n), row_ptr_(n + 1, 0) {
  for (unsigned long i = 0; i < size_; ++i) {
    set(i, i, v);
  }
}
arta::linalg::Matrix::Matrix(const Matrix& copy)
    : size_(copy.size_),
      row_ptr_(copy.row_ptr_),
      col_ind_(copy.col_ind_),
      vals_(copy.vals_) {}
double& arta::linalg::Matrix::operator()(unsigned long r, unsigned long c) {
  if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
    vals_.insert(vals_.begin() + row_ptr_[r + 1], 0.0);
    col_ind_.insert(col_ind_.begin() + row_ptr_[r], c);
    for (unsigned long i = r + 1; i < size_ + 1; ++i) {
      row_ptr_[i]++;
    }
    return vals_[row_ptr_[r]];
  } else {
    for (unsigned long i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
      if (col_ind_[i] == c) {
        return vals_[i];
      }
    }
    vals_.insert(vals_.begin() + row_ptr_[r + 1], 0.0);
    col_ind_.insert(col_ind_.begin() + row_ptr_[r + 1], c);
    for (unsigned long i = r + 1; i < size_ + 1; ++i) {
      row_ptr_[i]++;
    }
    return vals_[row_ptr_[r + 1] - 1];
  }
}
double arta::linalg::Matrix::operator()(unsigned long r, unsigned long c) const {
  if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
    return 0.0;
  } else {
    for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
      if (col_ind_[i] == c) {
        return vals_[i];
      }
    }
    return 0.0;
  }
}
double arta::linalg::Matrix::at(unsigned long r, unsigned long c) const {
  if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
    return 0.0;
  } else {
    for (std::size_t i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
      if (col_ind_[i] == c) {
        return vals_[i];
      }
    }
    return 0.0;
  }
}
void arta::linalg::Matrix::set(unsigned long r, unsigned long c,
                             const double& val) {
  if (row_ptr_[r + 1] - row_ptr_[r] == 0) {
    if (val != 0) {
      vals_.insert(vals_.begin() + row_ptr_[r + 1], val);
      col_ind_.insert(col_ind_.begin() + row_ptr_[r], c);
      for (unsigned long i = r + 1; i < size_ + 1; ++i) {
        row_ptr_[i]++;
      }
    }
    return;
  } else {
    for (unsigned long i = row_ptr_[r]; i < row_ptr_[r + 1]; ++i) {
      if (col_ind_[i] == c) {
        vals_[i] = val;
        if (val == 0) {
          vals_.erase(vals_.begin() + i);
          col_ind_.erase(col_ind_.begin() + i);
          for (unsigned long i = r + 1; i < size_ + 1; ++i) {
            row_ptr_[i]--;
          }
        }
        return;
      }
    }
    if (val != 0) {
      vals_.insert(vals_.begin() + row_ptr_[r + 1], val);
      col_ind_.insert(col_ind_.begin() + row_ptr_[r + 1], c);
      for (unsigned long i = r + 1; i < size_ + 1; ++i) {
        row_ptr_[i]++;
      }
    }
    return;
  }
}
void arta::linalg::Matrix::clear() {
  row_ptr_ = std::vector<unsigned long>(size_ + 1, 0);
  col_ind_.clear();
  vals_.clear();
}

std::string arta::linalg::Matrix::dump() const {
  std::string str;
  for (int r = 0; r < size_; ++r) {
    for (int c = 0; c < size_; ++c) {
      str += arta::fmt_val(at(r, c));
      if (c < size_ - 1) {
        str += " ";
      }
    }
    if (r < size_ - 1) {
      str += "\n";
    }
  }
  return str;
}

arta::linalg::Vector arta::linalg::operator*(const Matrix& lhs, const Vector& rhs) {
  Vector res(lhs.size());
  for (unsigned long r = 0; r < lhs.size(); ++r) {
    double sum = 0.0;
    for (unsigned long c = 0; c < rhs.size() && c < lhs.size(); ++c) {
      sum += (lhs.at(r, c) * rhs.at(c));
    }
    res.set(r, sum);
  }
  return res;
}

void arta::linalg::save_mat_to_file(const std::string& file_name,
                                  const Matrix& mat) {
  FILE* out = fopen(file_name.c_str(), "w");
  if (!out) {
    log::warning("Failed to open file \"%s\"", file_name.c_str());
    return;
  }
  fprintf(out, "%lu %lu\n", mat.size(), mat.count());
  const std::vector<unsigned long>* ulv = mat.get_row_ptr();
  for (unsigned long i = 0; i < ulv->size(); ++i) {
    fprintf(out, "%lu ", ulv->at(i));
  }
  fprintf(out, "\n");
  ulv = mat.get_col_ind();
  for (unsigned long i = 0; i < ulv->size(); ++i) {
    fprintf(out, "%lu ", ulv->at(i));
  }
  fprintf(out, "\n");
  const std::vector<double>* dv = mat.get_vals();
  for (unsigned long i = 0; i < dv->size(); ++i) {
    fprintf(out, "%lf ", dv->at(i));
  }
  fclose(out);
}

arta::linalg::Matrix arta::linalg::load_mat_from_file(
    const std::string& file_name) {
  FILE* src = fopen(file_name.c_str(), "r");
  if (!src) {
    log::warning("Failed to open file \"%s\"", file_name.c_str());
    return Matrix();
  }
  unsigned long size, n_vals;
  fscanf(src, "%lu %lu", &size, &n_vals);
  Matrix mat(size);
  std::vector<unsigned long>* row_ptr = mat.get_row_ptr();
  std::vector<unsigned long>* col_ind = mat.get_col_ind();
  std::vector<double>* vals = mat.get_vals();
  row_ptr->clear();
  for (unsigned long i = 0; i <= size; ++i) {
    unsigned long rp;
    fscanf(src, "%lu", &rp);
    row_ptr->push_back(rp);
  }
  for (unsigned long i = 0; i < n_vals; ++i) {
    unsigned long ci;
    fscanf(src, "%lu", &ci);
    col_ind->push_back(ci);
  }
  for (unsigned long i = 0; i < n_vals; ++i) {
    double v;
    fscanf(src, "%lf", &v);
    vals->push_back(v);
  }
  fclose(src);
  return mat;
}
