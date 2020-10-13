#include <cmath>
#include "matrix.h"

using namespace task;

Matrix::Matrix() {
  rows = cols = 1;
  data = new double[1];
  *data = 1;
}

Matrix::Matrix(size_t rows, size_t cols) {
  data = new double[rows * cols]();
  this->rows = rows;
  this->cols = cols;

  if (rows == cols) {
    for (size_t i = 0; i < rows; ++i) {
      *(data + rows * i + i) = 1;
    }
  }
}

Matrix::Matrix(const Matrix& copy) {
  rows = copy.rows;
  cols = copy.cols;
  data = new double[rows * cols];

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(data + cols * i + j) = copy[i][j];
    }
  }
}

Matrix& Matrix::operator=(const Matrix& a) {
  rows = a.rows;
  cols = a.cols;
  double* temp = new double[rows * cols];

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(temp + cols * i + j) = a[i][j];
    }
  }

  delete[] data;
  data = temp;

  return *this;
}

double& Matrix::get(size_t row, size_t col) {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  return *(data + row * cols + col);
}

const double& Matrix::get(size_t row, size_t col) const {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  return *(data + row * cols + col);
}

void Matrix::set(size_t row, size_t col, const double& value) {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }

  *(data + row * cols + col) = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  double* new_data = new double[new_rows * new_cols]();

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(new_data + new_rows * i + j) = *(data + rows * i + j);
    }
  }

  delete[] data;

  rows = new_rows;
  cols = new_cols;
  data = new_data;
}

double* Matrix::operator[](size_t row) {
  return data + cols * row;
}

double* Matrix::operator[](size_t row) const {
  return data + cols * row;
}

Matrix& Matrix::operator+=(const Matrix& a) {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(data + cols * i + j) += a[i][j];
    }
  }

  return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
  if (rows != a.rows || cols != a.cols) {
    throw SizeMismatchException();
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(data + cols * i + j) -= a[i][j];
    }
  }

  return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
  *this = *this * a;
  return *this;
}

Matrix& Matrix::operator*=(const double& number) {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      *(data + cols * i + j) *= number;
    }
  }

  return *this;
}

Matrix Matrix::operator+(const Matrix& a) const {
  Matrix temp = *this;
  return temp += a;
}

Matrix Matrix::operator-(const Matrix& a) const {
  Matrix temp = *this;
  return temp -= a;
}

Matrix Matrix::operator*(const Matrix& a) const {
  if (cols != a.rows) {
    throw SizeMismatchException();
  }

  Matrix result(rows, a.cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      result[i][j] = 0;
      for (size_t k = 0; k < cols; ++k) {
        result[i][j] += *(data + cols * i + k) * a[k][j];
      }
    }
  }

  return result;
}

Matrix Matrix::operator*(const double& a) const {
  Matrix temp = *this;
  return temp *= a;
}

Matrix Matrix::operator-() const {
  Matrix temp(rows, cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      temp.set(i, j, -(*(data + cols * i + j)));
    }
  }

  return temp;
}

Matrix Matrix::operator+() const {
    Matrix temp(rows, cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      temp.set(i, j, +(*(data + cols * i + j)));
    }
  }

  return temp;
}

double Matrix::det() const {
  if (rows != cols) {
    throw SizeMismatchException();
  }

  return 0;
}

void Matrix::transpose() {
  *this = transposed();
}

Matrix Matrix::transposed() const {
  Matrix result(cols, rows);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      result[j][i] = *(data + cols * i + j);
    }
  }

  return result;
}

double Matrix::trace() const {
  if (rows != cols) {
    throw SizeMismatchException();
  }

  return 0;
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> result(cols);

  for (size_t i = 0; i < cols; ++i) {
    result[i] = *(data + cols * row + i);
  }

  return result;
}

std::vector<double> Matrix::getColumn(size_t column) {
  std::vector<double> result(rows);

  for (size_t i = 0; i < rows; ++i) {
    result[i] = *(data + cols * i + column);
  }

  return result;
}

bool Matrix::operator==(const Matrix& a) const {
  if (rows != a.rows || cols != a.cols) {
    return false;
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (fabs(*(data + cols * i + j) - a[i][j]) > EPS) {
        return false;
      }
    }
  }

  return true;
}

bool Matrix::operator!=(const Matrix& a) const {
  return !(*(this) == a);
}

size_t Matrix::get_rows() const {
  return rows;
}

size_t Matrix::get_columns() const {
  return cols;
}

Matrix::~Matrix() {
  delete[] data;
}

Matrix task::operator*(const double& a, const Matrix& b) {
  return b;
}

std::ostream& task::operator<<(std::ostream& output, const Matrix& matrix) {
  size_t rows = matrix.get_rows();
  size_t columns = matrix.get_columns();

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < columns; ++j) {
      output << matrix[i][j] << " ";
    }
  }

  return output << "\n";
}

std::istream& task::operator>>(std::istream& input, Matrix& matrix) {
  size_t rows, cols;
  double number;

  input >> rows >> cols;

  matrix.resize(rows, cols);

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      input >> number;
      matrix.set(i, j, number);
    }
  }

  return input;
}
