#ifndef FEM_MATH_MATRIX_HPP_
#define FEM_MATH_MATRIX_HPP_

#include <string>
#include <vector>

namespace fem {
namespace math {
  class Matrix {
   public:
    Matrix();
    explicit Matrix(unsigned long n);
    Matrix(unsigned long n, const double& v);
    Matrix(const Matrix& mat);

    inline unsigned long size() const noexcept { return size_; }
    inline unsigned long count() const noexcept { return vals_.size(); }

    double& operator()(unsigned long r, unsigned long c);
    double operator()(unsigned long r, unsigned long c) const;
    double at(unsigned long r, unsigned long c) const;
    void set(unsigned long r, unsigned long c, const double& val);

    void clear();

    std::vector<unsigned long>* get_row_ptr() { return &row_ptr_; }
    std::vector<unsigned long>* get_col_ind() { return &col_ind_; }
    std::vector<double>* get_vals() { return &vals_; }
    const std::vector<unsigned long>* get_row_ptr() const { return &row_ptr_; }
    const std::vector<unsigned long>* get_col_ind() const { return &col_ind_; }
    const std::vector<double>* get_vals() const { return &vals_; }

   private:
    unsigned long size_;
    std::vector<unsigned long> row_ptr_, col_ind_;
    std::vector<double> vals_;
  };

  void save_mat_to_file(const std::string& file_name, const Matrix& mat);
  Matrix load_mat_from_file(const std::string& file_name);
}  // namespace math
}  // namespace fem

#endif  // FEM_MATH_MATRIX_HPP_
