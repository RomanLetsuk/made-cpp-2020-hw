#pragma once
#include <vector>
#include <iostream>

using std::vector;

namespace task {
  vector<double> operator+(vector<double> a, vector<double> b);
  vector<double> operator-(vector<double> a, vector<double> b);
  vector<double> operator+(vector<double> a);
  vector<double> operator-(vector<double> a);
  double operator*(vector<double> a, vector<double> b);
  vector<double> operator%(vector<double> a, vector<double> b);
  bool operator||(vector<double> a, vector<double> b);
  bool operator&&(vector<double> a, vector<double> b);
  std::istream& operator>>(std::istream &in, vector<double> &v);
  std::ostream& operator<<(std::ostream &os, vector<double> &v);
  void reverse(vector<double> &a);
  vector<int> operator|(vector<int> a, vector<int> b);
  vector<int> operator&(vector<int> a, vector<int> b);
}  // namespace task
