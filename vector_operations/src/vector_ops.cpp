#include <vector>
#include <iostream>
#include <cmath>

using std::vector;
double EPS = 1E-7;

void swap(double& a, double& b) {
  double temp = a;
  a = b;
  b = temp;
}

namespace task {
  vector<double> operator+(vector<double> a, vector<double> b) {
    size_t size = a.size();
    vector<double> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = a[i] + b[i];
    }

    return result;
  }

  vector<double> operator-(vector<double> a, vector<double> b) {
    size_t size = a.size();
    vector<double> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = a[i] - b[i];
    }

    return result;
  }

  vector<double> operator+(vector<double> a) {
    size_t size = a.size();
    vector<double> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = +a[i];
    }

    return result;
  }

  vector<double> operator-(vector<double> a) {
    size_t size = a.size();
    vector<double> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = -a[i];
    }

    return result;
  }

  double operator*(vector<double> a, vector<double> b) {
    size_t size = a.size();
    double result = 0;

    for (size_t i = 0; i < size; i++) {
      result += a[i] * b[i];
    }

    return result;
  }

  vector<double> operator%(vector<double> a, vector<double> b) {
    size_t size = a.size();
    vector<double> result(size);

    result[0] = a[2] * b[1] - a[1] * b[2];
    result[1] = a[0] * b[2] - a[2] * b[0];
    result[2] = a[1] * b[0] - a[0] * b[1];

    return result;
  }

  bool operator||(vector<double> a, vector<double> b) {
    size_t size = a.size();
    bool result = true;

    for (size_t i = 1; i < size; i++) {
      result &= fabs(a[i - 1] * b[i] - a[i] * b[i - 1]) < EPS;
    }

    return result;
  }

  bool operator&&(vector<double> a, vector<double> b) {
    if (!(a || b)) {
      return false;
    }

    size_t size = a.size();

    for (size_t i = 0; i < size; i++) {
      if (a[i] != 0 && b[i] != 0) {
        return a[i] / b[i] > 0;
      }
    }

    return true;
  }

  std::istream& operator>>(std::istream &in, vector<double> &v) {
    size_t size;
    in >> size;
    v.resize(size);

    for (size_t i = 0; i < size; i++) {
      in >> v[i];
    }

    return in;
  }

  std::ostream& operator<<(std::ostream &out, vector<double> &v) {
    size_t size = v.size();

    for (size_t i = 0; i < size; i++) {
      out << v[i] << " ";
    }

    return out << "\n";
  }

  void reverse(vector<double> &a) {
    size_t size = a.size();

    for (size_t i = 0; i < size / 2; i++) {
      swap(a[i], a[size - i - 1]);
    }
  }

  vector<int> operator|(vector<int> a, vector<int> b) {
    size_t size = a.size();
    vector<int> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = a[i] | b[i];
    }

    return result;
  }

  vector<int> operator&(vector<int> a, vector<int> b) {
    size_t size = a.size();
    vector<int> result(size);

    for (size_t i = 0; i < size; i++) {
      result[i] = a[i] & b[i];
    }

    return result;
  }
}
