#pragma once
#include<vector>

struct Point {
public:
  double x;
  double y;

  Point(double x, double y);
  void rotate(Point center, double angle);
  void scale(Point center, double coefficient);
  bool operator==(const Point&) const;
  bool operator!=(const Point&) const;
};

class Line {
public:
  double k;
  double b;

  Line(Point a, Point b);
  Line(double k, double b);
  Line(Point a, double k);
  void rotate(Point center, double angle);
  bool operator==(const Line&) const;
  bool operator!=(const Line&) const;
};

class Shape {
protected:
  virtual std::vector<Point>& getPoints() = 0;
public:
  virtual double perimeter() const = 0;
  virtual double area() const = 0;
  virtual bool operator==(const Shape&) const = 0;
  virtual bool operator!=(const Shape&) const = 0;
  void rotate(Point center, double angle);
  void reflex(Point center);
  void reflex(Line axis);
  void scale(Point center, double coefficient);
};

class Polygon : public Shape {
private:
  std::vector<Point> vertices;
protected:
  void setVertices(std::vector<Point>);
  std::vector<Point>& getPoints() override;
public:
  Polygon();
  Polygon(std::vector<Point>);
  int verticesCount() const;
  const std::vector<Point> getVertices() const;
  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};

class Ellipse : public Shape {
private:
  Point f1;
  Point f2;
  double a;
  double b;
protected:
  std::vector<Point>& getPoints() override;
public:
  Ellipse(Point, Point, double);
  const std::pair<Point, Point> focuses() const;
  double eccentricity() const;
  double perimeter() const override;
  double area() const override;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};

class Circle : public Ellipse {
private:
  Point c;
  double r;
protected:
  std::vector<Point>& getPoints() override;
public:
  Circle(Point center, double radius);
  double radius() const;
  const Point center() const;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};

class Rectangle : public Polygon {
private:
  Point c;
public:
  Rectangle(Point, Point, double ratio);
  const Point center() const;
  const std::pair<Line, Line> diagonals() const;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};

class Square : public Rectangle {
public:
  Square(Point, Point);
  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};

class Triangle : public Polygon {
public:
  Triangle(Point, Point, Point);
  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
  Point centroid() const;
  Point orthocenter() const;
  Line EulerLine() const;
  Circle ninePointsCircle() const;
  bool operator==(const Shape&) const override;
  bool operator!=(const Shape&) const override;
};
